/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "UsbCameraImpl.h"

#include <ks.h>
#include <ksmedia.h>
#include <mfapi.h>
#include <mferror.h>
#include <mfidl.h>
#include <shlwapi.h>
#include <windowsx.h>

#include <cmath>
#include <codecvt>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <Dbt.h>
#include <Dshow.h>
#include <Windows.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <wpi/SmallString.h>
#include <wpi/memory.h>
#include <wpi/raw_ostream.h>
#include <wpi/timestamp.h>

#include "COMCreators.h"
#include "ComPtr.h"
#include "GUIDName.h"
#include "Handle.h"
#include "JpegUtil.h"
#include "Log.h"
#include "Notifier.h"
#include "PropertyImpl.h"
#include "Telemetry.h"
#include "WindowsMessagePump.h"
#include "c_util.h"
#include "cscore_cpp.h"

#pragma comment(lib, "Mfplat.lib")
#pragma comment(lib, "Mf.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "Shlwapi.lib")

static constexpr int NewImageMessage = 0x0400 + 4488;
static constexpr int SetCameraMessage = 0x0400 + 254;

static constexpr unsigned kPropConnectVerboseId = 0;

using namespace cs;

namespace cs {

UsbCameraImpl::UsbCameraImpl(const wpi::Twine& name, const wpi::Twine& path)
    : SourceImpl{name}, m_path{path.str()} {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
  m_widePath = utf8_conv.from_bytes(m_path.c_str());
}

UsbCameraImpl::UsbCameraImpl(const wpi::Twine& name, int deviceId)
    : SourceImpl{name}, m_deviceId(deviceId) {}

UsbCameraImpl::~UsbCameraImpl() { m_messagePump = nullptr; }

void UsbCameraImpl::SetProperty(int property, int value, CS_Status* status) {
  Message msg{Message::kCmdSetProperty};
  msg.data[0] = property;
  msg.data[1] = value;
  auto result =
      m_messagePump->SendWindowMessage<CS_Status, Message::Kind, Message*>(
          SetCameraMessage, msg.kind, &msg);
  *status = result;
}
void UsbCameraImpl::SetStringProperty(int property, const wpi::Twine& value,
                                      CS_Status* status) {
  Message msg{Message::kCmdSetPropertyStr};
  msg.data[0] = property;
  msg.dataStr = value.str();
  auto result =
      m_messagePump->SendWindowMessage<CS_Status, Message::Kind, Message*>(
          SetCameraMessage, msg.kind, &msg);
  *status = result;
}

// Standard common camera properties
void UsbCameraImpl::SetBrightness(int brightness, CS_Status* status) {}
int UsbCameraImpl::GetBrightness(CS_Status* status) const { return 0; }
void UsbCameraImpl::SetWhiteBalanceAuto(CS_Status* status) {}
void UsbCameraImpl::SetWhiteBalanceHoldCurrent(CS_Status* status) {}
void UsbCameraImpl::SetWhiteBalanceManual(int value, CS_Status* status) {}
void UsbCameraImpl::SetExposureAuto(CS_Status* status) {}
void UsbCameraImpl::SetExposureHoldCurrent(CS_Status* status) {}
void UsbCameraImpl::SetExposureManual(int value, CS_Status* status) {}

bool UsbCameraImpl::SetVideoMode(const VideoMode& mode, CS_Status* status) {
  std::cout << "Setting Video Mode Before Check\n";
  if (mode.pixelFormat == VideoMode::kUnknown) {
    *status = CS_UNSUPPORTED_MODE;
    return false;
  }

  std::cout << "Setting Video Mode\n";
  Message msg{Message::kCmdSetMode};
  msg.data[0] = mode.pixelFormat;
  msg.data[1] = mode.width;
  msg.data[2] = mode.height;
  msg.data[3] = mode.fps;
  auto result =
      m_messagePump->SendWindowMessage<CS_Status, Message::Kind, Message*>(
          SetCameraMessage, msg.kind, &msg);
  *status = result;
  return result == 0;
}

bool UsbCameraImpl::SetPixelFormat(VideoMode::PixelFormat pixelFormat,
                                   CS_Status* status) {
  if (pixelFormat == VideoMode::kUnknown) {
    *status = CS_UNSUPPORTED_MODE;
    return false;
  }
  std::cout << "Setting Pixel Format Mode\n";
  Message msg{Message::kCmdSetPixelFormat};
  msg.data[0] = pixelFormat;
  auto result =
      m_messagePump->SendWindowMessage<CS_Status, Message::Kind, Message*>(
          SetCameraMessage, msg.kind, &msg);
  *status = result;
  return result == 0;
}
bool UsbCameraImpl::SetResolution(int width, int height, CS_Status* status) {
  Message msg{Message::kCmdSetResolution};
  msg.data[0] = width;
  msg.data[1] = height;
  auto result =
      m_messagePump->SendWindowMessage<CS_Status, Message::Kind, Message*>(
          SetCameraMessage, msg.kind, &msg);
  *status = result;
  return result == 0;
}
bool UsbCameraImpl::SetFPS(int fps, CS_Status* status) {
  Message msg{Message::kCmdSetFPS};
  msg.data[0] = fps;
  auto result =
      m_messagePump->SendWindowMessage<CS_Status, Message::Kind, Message*>(
          SetCameraMessage, msg.kind, &msg);
  *status = result;
  return result == 0;
}

void UsbCameraImpl::NumSinksChanged() {
  m_messagePump->PostWindowMessage<Message::Kind, Message*>(
      SetCameraMessage, Message::kNumSinksChanged, nullptr);
}
void UsbCameraImpl::NumSinksEnabledChanged() {
  m_messagePump->PostWindowMessage<Message::Kind, Message*>(
      SetCameraMessage, Message::kNumSinksEnabledChanged, nullptr);
}

void UsbCameraImpl::Start() {
  m_messagePump = std::make_unique<WindowsMessagePump>(
      [this](HWND hwnd, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
        return this->PumpMain(hwnd, uiMsg, wParam, lParam);
      });
}

void UsbCameraImpl::PostRequestNewFrame() {
  m_messagePump->PostWindowMessage(NewImageMessage, nullptr, nullptr);
}

bool UsbCameraImpl::CheckDeviceChange(WPARAM wParam, DEV_BROADCAST_HDR* pHdr,
                                      bool* connected) {
  DEV_BROADCAST_DEVICEINTERFACE* pDi = NULL;

  *connected = false;

  if (pHdr == NULL) {
    return false;
  }
  if (pHdr->dbch_devicetype != DBT_DEVTYP_DEVICEINTERFACE) {
    return false;
  }

  // Compare the device name with the symbolic link.

  pDi = reinterpret_cast<DEV_BROADCAST_DEVICEINTERFACE*>(pHdr);

  std::cout << "Device change check\n";

  std::cout << "DC'd: " << pDi->dbcc_name << "\n";
  std::cout << "Path: " << m_path << "\n";

  if (_stricmp(m_path.c_str(), pDi->dbcc_name) == 0) {
    std::cout << "Strcmp true\n";
    if (wParam == DBT_DEVICEARRIVAL) {
      *connected = true;
      std::cout << "rettrue\n";
      return true;
    } else if (wParam == DBT_DEVICEREMOVECOMPLETE) {
      *connected = false;
      std::cout << "rettrue\n";
      return true;
    }
  }
  return false;
}

void UsbCameraImpl::DeviceDisconnect() {
  if (m_connectVerbose) SINFO("Disconnected from " << m_path);
  m_sourceReader.Reset();
  m_mediaSource.Reset();
  SetConnected(false);
}

void UsbCameraImpl::ProcessFrame(IMFSample* videoSample) {

  do {
    if (!videoSample) break;

    ComPtr<IMFMediaBuffer> buf;

    if (!SUCCEEDED(
            videoSample->ConvertToContiguousBuffer(buf.GetAddressOf()))) {
      DWORD bcnt = 0;
      if (!SUCCEEDED(videoSample->GetBufferCount(&bcnt))) break;
      if (bcnt == 0) break;
      if (!SUCCEEDED(videoSample->GetBufferByIndex(0, buf.GetAddressOf())))
        break;
    }

    bool lock2d = false;
    BYTE* ptr = NULL;
    LONG pitch = 0;
    DWORD maxsize = 0, cursize = 0;

    // "For 2-D buffers, the Lock2D method is more efficient than the Lock
    // method" see IMFMediaBuffer::Lock method documentation:
    // https://msdn.microsoft.com/en-us/library/windows/desktop/bb970366(v=vs.85).aspx
    ComPtr<IMF2DBuffer> buffer2d;
    DWORD memLength2d = 0;
    if (true) {
      buffer2d = buf.As<IMF2DBuffer>();
      if (buffer2d) {
        buffer2d->GetContiguousLength(&memLength2d);
        if (SUCCEEDED(buffer2d->Lock2D(&ptr, &pitch))) {
          lock2d = true;
        }
      }
    }
    if (ptr == NULL) {
      if (!SUCCEEDED(buf->Lock(&ptr, &maxsize, &cursize))) {
        break;
      }
    }
    if (!ptr) break;

    cv::Mat tmpMat;
    std::unique_ptr<Image> dest;
    bool doFinalSet = true;

    VideoMode mode;
    {
      std::unique_lock<wpi::mutex> lock(m_mutex);
      mode = m_mode;
    }

    switch (mode.pixelFormat) {
      case cs::VideoMode::PixelFormat::kMJPEG: {
        // Special case
        PutFrame(VideoMode::kMJPEG, mode.width, mode.height,
                 wpi::StringRef(reinterpret_cast<char*>(ptr), cursize),
                 wpi::Now());
        doFinalSet = false;
        break;
      }
      case cs::VideoMode::PixelFormat::kGray:
        tmpMat = cv::Mat(mode.height, mode.width, CV_8UC1, ptr, pitch);
        dest = AllocImage(VideoMode::kGray, tmpMat.cols, tmpMat.rows,
                          tmpMat.total());
        tmpMat.copyTo(dest->AsMat());
        break;
      case cs::VideoMode::PixelFormat::kBGR:
        tmpMat = cv::Mat(mode.height, mode.width, CV_8UC3, ptr, pitch);
        dest = AllocImage(VideoMode::kBGR, tmpMat.cols, tmpMat.rows,
                          tmpMat.total() * 3);
        tmpMat.copyTo(dest->AsMat());
        break;
      case cs::VideoMode::PixelFormat::kYUYV:
        tmpMat = cv::Mat(mode.height, mode.width, CV_8UC2, ptr, pitch);
        dest = AllocImage(VideoMode::kYUYV, tmpMat.cols, tmpMat.rows,
                          tmpMat.total() * 2);
        tmpMat.copyTo(dest->AsMat());
        break;
      default:
        std::cout << "default case\n";
        doFinalSet = false;
        break;
    }

    if (doFinalSet) {
      PutFrame(std::move(dest), wpi::Now());
    }

    if (lock2d)
      buffer2d->Unlock2D();
    else
      buf->Unlock();
  } while (0);
}

LRESULT UsbCameraImpl::PumpMain(HWND hwnd, UINT uiMsg, WPARAM wParam,
                                LPARAM lParam) {
  switch (uiMsg) {
    case WM_CLOSE:
      m_sourceReader.Reset();
      m_mediaSource.Reset();
      m_imageCallback.Reset();
      break;
    case WM_CREATE:
      // Pump Created and ready to go
      m_imageCallback = CreateSourceReaderCB(this);
      DeviceConnect();
      break;
    case WM_DEVICECHANGE: {
      // Device potentially changed
      PDEV_BROADCAST_HDR parameter =
          reinterpret_cast<PDEV_BROADCAST_HDR>(lParam);
      // Check if we're waiting on a device path, and this is a connection
      if (m_path.empty() && wParam == DBT_DEVICEARRIVAL &&
          parameter->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE) {
        // If path is empty, we attempted to connect with a device ID. Enumerate
        // and check
        CS_Status status = 0;
        auto devices = cs::EnumerateUsbCameras(&status);
        if (devices.size() > m_deviceId) {
          // If has device ID, use the device ID from the event
          // because of windows bug
          auto&& device = devices[m_deviceId];
          DEV_BROADCAST_DEVICEINTERFACE* pDi =
              reinterpret_cast<DEV_BROADCAST_DEVICEINTERFACE*>(parameter);
          m_path = pDi->dbcc_name;
          std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
          m_widePath = utf8_conv.from_bytes(m_path.c_str());
        } else {
          // This device not found
          break;
        }
      }
      bool connected = false;
      if (CheckDeviceChange(wParam, parameter, &connected)) {
        if (connected) {
          DeviceConnect();
        } else {
          // Disconnected
          DeviceDisconnect();
        }
      }
    } break;
    case NewImageMessage: {  // New image
      if (m_streaming && m_sourceReader) {
        m_sourceReader->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, NULL,
                                   NULL, NULL, NULL);
      }
      break;
    }
    case SetCameraMessage: {
      {
        std::cout << "Set Camera Message\n";
        Message* msg = reinterpret_cast<Message*>(lParam);
        Message::Kind msgKind = static_cast<Message::Kind>(wParam);
        std::unique_lock<wpi::mutex> lock(m_mutex);
        auto retVal = DeviceProcessCommand(lock, msgKind, msg);
        return retVal;
      }
      break;
    }
  }
  return 0l;
}

static cs::VideoMode::PixelFormat GetFromGUID(const GUID& guid) {
  // Compare GUID to one of the supported ones
  if (guid == MFVideoFormat_NV12) {
    // GrayScale
    return cs::VideoMode::PixelFormat::kGray;
  } else if (guid == MFVideoFormat_YUY2) {
    return cs::VideoMode::PixelFormat::kYUYV;
  } else if (guid == MFVideoFormat_RGB24) {
    return cs::VideoMode::PixelFormat::kBGR;
  } else if (guid == MFVideoFormat_MJPG) {
    return cs::VideoMode::PixelFormat::kMJPEG;
  } else if (guid == MFVideoFormat_RGB565) {
    return cs::VideoMode::PixelFormat::kRGB565;
  } else {
    return cs::VideoMode::PixelFormat::kUnknown;
  }
}

bool UsbCameraImpl::DeviceConnect() {
  if (m_mediaSource && m_sourceReader) return true;

  if (m_connectVerbose) SINFO("Connecting to USB camera on " << m_path);

  SDEBUG3("opening device");

  const wchar_t* path = m_widePath.c_str();
  m_mediaSource = CreateVideoCaptureDevice(path);

  if (!m_mediaSource) return false;

  m_sourceReader =
      CreateSourceReader(m_mediaSource.Get(), m_imageCallback.Get());

  if (!m_sourceReader) {
    m_mediaSource.Reset();
    return false;
  }

  if (!m_properties_cached) {
    SDEBUG3("caching properties");
    DeviceCacheProperties();
    DeviceCacheVideoModes();
    DeviceCacheMode();
    m_properties_cached = true;
  } else {
    SDEBUG3("restoring video mode");
    DeviceSetMode();
  }

  SetConnected(true);

  std::cout << "Setting Stream: " << m_streaming << " " << IsEnabled()
            << std::endl;



  // Turn off streaming if not enabled, and turn it on if enabled
  if (m_streaming && !IsEnabled()) {
    DeviceStreamOff();
  } else if (IsEnabled()) {
    std::cout << "Setting Stream On" << std::endl;
    std::cout << m_streaming << std::endl;
    std::cout << m_deviceValid << std::endl;
    DeviceStreamOn();
  }
  return true;
}

std::unique_ptr<PropertyImpl> UsbCameraImpl::CreateEmptyProperty(
    const wpi::Twine& name) const {
  return nullptr;
}

bool UsbCameraImpl::CacheProperties(CS_Status* status) const { return true; }

void UsbCameraImpl::DeviceAddProperty(const wpi::Twine& name_,
                                      tagVideoProcAmpProperty tag,
                                      IAMVideoProcAmp* pProcAmp) {
  // First see if properties exist
  bool isValid = false;
  auto property = std::make_unique<UsbCameraProperty>(name_, tag, false,
                                                      pProcAmp, &isValid);
  if (isValid) {
    DeviceCacheProperty(std::move(property), pProcAmp);
  }
}

#define CREATEPROPERTY(val) \
  DeviceAddProperty(#val, VideoProcAmp_##val, pProcAmp);

void UsbCameraImpl::DeviceCacheProperties() {
  if (!m_sourceReader) return;

  IAMVideoProcAmp* pProcAmp = NULL;

  if (SUCCEEDED(m_sourceReader->GetServiceForStream(
          (DWORD)MF_SOURCE_READER_MEDIASOURCE, GUID_NULL,
          IID_PPV_ARGS(&pProcAmp)))) {
    CREATEPROPERTY(Brightness)
    CREATEPROPERTY(Contrast)
    CREATEPROPERTY(Hue)
    CREATEPROPERTY(Saturation)
    CREATEPROPERTY(Sharpness)
    CREATEPROPERTY(Gamma)
    CREATEPROPERTY(ColorEnable)
    CREATEPROPERTY(WhiteBalance)
    CREATEPROPERTY(BacklightCompensation)
    CREATEPROPERTY(Gain)
    pProcAmp->Release();
  }
}

int UsbCameraImpl::RawToPercentage(const UsbCameraProperty& rawProp,
                                   int rawValue) {
  return 100.0 * (rawValue - rawProp.minimum) /
         (rawProp.maximum - rawProp.minimum);
}

int UsbCameraImpl::PercentageToRaw(const UsbCameraProperty& rawProp,
                                   int percentValue) {
  return rawProp.minimum +
         (rawProp.maximum - rawProp.minimum) * (percentValue / 100.0);
}

void UsbCameraImpl::DeviceCacheProperty(
    std::unique_ptr<UsbCameraProperty> rawProp, IAMVideoProcAmp* pProcAmp) {
  // For percentage properties, we want to cache both the raw and the
  // percentage versions.  This function is always called with prop being
  // the raw property (as it's coming from the camera) so if required, we need
  // to rename this one as well as create/cache the percentage version.
  //
  // This is complicated by the fact that either the percentage version or the
  // the raw version may have been set previously.  If both were previously set,
  // the raw version wins.
  std::unique_ptr<UsbCameraProperty> perProp;
  if (false) {
    perProp =
        wpi::make_unique<UsbCameraProperty>(rawProp->name, 0, *rawProp, 0, 0);
    rawProp->name = "raw_" + perProp->name;
  }

  std::unique_lock<wpi::mutex> lock(m_mutex);
  int* rawIndex = &m_properties[rawProp->name];
  bool newRaw = *rawIndex == 0;
  UsbCameraProperty* oldRawProp =
      newRaw ? nullptr
             : static_cast<UsbCameraProperty*>(GetProperty(*rawIndex));

  int* perIndex = perProp ? &m_properties[perProp->name] : nullptr;
  bool newPer = !perIndex || *perIndex == 0;
  UsbCameraProperty* oldPerProp =
      newPer ? nullptr
             : static_cast<UsbCameraProperty*>(GetProperty(*perIndex));

  if (oldRawProp && oldRawProp->valueSet) {
    // Merge existing raw setting and set percentage from it
    rawProp->SetValue(oldRawProp->value);
    rawProp->valueStr = std::move(oldRawProp->valueStr);

    if (perProp) {
      perProp->SetValue(RawToPercentage(*rawProp, rawProp->value));
      perProp->valueStr = rawProp->valueStr;  // copy
    }
  } else if (oldPerProp && oldPerProp->valueSet) {
    // Merge existing percentage setting and set raw from it
    perProp->SetValue(oldPerProp->value);
    perProp->valueStr = std::move(oldPerProp->valueStr);

    rawProp->SetValue(PercentageToRaw(*rawProp, perProp->value));
    rawProp->valueStr = perProp->valueStr;  // copy
  } else {
    // Read current raw value and set percentage from it
    if (!rawProp->DeviceGet(lock, pProcAmp))
      SWARNING("failed to get property " << rawProp->name);

    if (perProp) {
      perProp->SetValue(RawToPercentage(*rawProp, rawProp->value));
      perProp->valueStr = rawProp->valueStr;  // copy
    }
  }

  // Set value on device if user-configured
  if (rawProp->valueSet) {
    if (!rawProp->DeviceSet(lock, pProcAmp))
      SWARNING("failed to set property " << rawProp->name);
  }

  // Update pointers since we released the lock
  rawIndex = &m_properties[rawProp->name];
  perIndex = perProp ? &m_properties[perProp->name] : nullptr;

  // Get pointers before we move the std::unique_ptr values
  auto rawPropPtr = rawProp.get();
  auto perPropPtr = perProp.get();

  if (newRaw) {
    // create a new index
    *rawIndex = m_propertyData.size() + 1;
    m_propertyData.emplace_back(std::move(rawProp));
  } else {
    // update
    m_propertyData[*rawIndex - 1] = std::move(rawProp);
  }

  // Finish setting up percentage property
  if (perProp) {
    perProp->propPair = *rawIndex;
    perProp->defaultValue =
        RawToPercentage(*rawPropPtr, rawPropPtr->defaultValue);

    if (newPer) {
      // create a new index
      *perIndex = m_propertyData.size() + 1;
      m_propertyData.emplace_back(std::move(perProp));
    } else if (perIndex) {
      // update
      m_propertyData[*perIndex - 1] = std::move(perProp);
    }

    // Tell raw property where to find percentage property
    rawPropPtr->propPair = *perIndex;
  }

  NotifyPropertyCreated(*rawIndex, *rawPropPtr);
  if (perPropPtr) NotifyPropertyCreated(*perIndex, *perPropPtr);
}

CS_StatusValue UsbCameraImpl::DeviceProcessCommand(
    std::unique_lock<wpi::mutex>& lock, Message::Kind msgKind,
    const Message* msg) {
  if (msgKind == Message::kCmdSetMode ||
      msgKind == Message::kCmdSetPixelFormat ||
      msgKind == Message::kCmdSetResolution || msgKind == Message::kCmdSetFPS) {
    return DeviceCmdSetMode(lock, *msg);
  } else if (msgKind == Message::kCmdSetProperty ||
             msgKind == Message::kCmdSetPropertyStr) {
    return DeviceCmdSetProperty(lock, *msg);
    return CS_OK;
  } else if (msgKind == Message::kNumSinksChanged ||
             msgKind == Message::kNumSinksEnabledChanged) {
    std::cout << "Stream Sink Event\n";
    // Turn On Streams
    if (m_streaming && !IsEnabled()) {
      DeviceStreamOff();
    } else if (!m_streaming && IsEnabled()) {
      DeviceStreamOn();
    }
    return CS_OK;
  } else {
    return CS_OK;
  }
}

CS_StatusValue UsbCameraImpl::DeviceCmdSetProperty(
    std::unique_lock<wpi::mutex>& lock, const Message& msg) {
  bool setString = (msg.kind == Message::kCmdSetPropertyStr);
  int property = msg.data[0];
  int value = msg.data[1];
  wpi::StringRef valueStr = msg.dataStr;

  // Look up
  auto prop = static_cast<UsbCameraProperty*>(GetProperty(property));
  if (!prop) return CS_INVALID_PROPERTY;

  // If setting before we get, guess initial type based on set
  if (prop->propKind == CS_PROP_NONE) {
    if (setString)
      prop->propKind = CS_PROP_STRING;
    else
      prop->propKind = CS_PROP_INTEGER;
  }

  // Check kind match
  if ((setString && prop->propKind != CS_PROP_STRING) ||
      (!setString && (prop->propKind &
                      (CS_PROP_BOOLEAN | CS_PROP_INTEGER | CS_PROP_ENUM)) == 0))
    return CS_WRONG_PROPERTY_TYPE;

  // Handle percentage property
  int percentageProperty = prop->propPair;
  int percentageValue = value;
  if (percentageProperty != 0) {
    if (prop->percentage) {
      std::swap(percentageProperty, property);
      prop = static_cast<UsbCameraProperty*>(GetProperty(property));
      value = PercentageToRaw(*prop, percentageValue);
    } else {
      percentageValue = RawToPercentage(*prop, value);
    }
  }

  // Actually set the new value on the device (if possible)
  if (!prop->device) {
    if (prop->id == kPropConnectVerboseId) m_connectVerbose = value;
  } else {
    IAMVideoProcAmp* pProcAmp = NULL;
    if (SUCCEEDED(m_sourceReader->GetServiceForStream(
            (DWORD)MF_SOURCE_READER_MEDIASOURCE, GUID_NULL,
            IID_PPV_ARGS(&pProcAmp)))) {
      if (!prop->DeviceSet(lock, pProcAmp, value)) {
        pProcAmp->Release();
        return CS_PROPERTY_WRITE_FAILED;
      }
      pProcAmp->Release();
    } else {
      return CS_PROPERTY_WRITE_FAILED;
    }
  }

  // Cache the set values
  UpdatePropertyValue(property, setString, value, valueStr);
  if (percentageProperty != 0)
    UpdatePropertyValue(percentageProperty, setString, percentageValue,
                        valueStr);

  return CS_OK;
}

ComPtr<IMFMediaType> UsbCameraImpl::DeviceCheckModeValid(
    const VideoMode& toCheck) {
  // Find the matching mode
  auto match =
      std::find_if(m_windowsVideoModes.begin(), m_windowsVideoModes.end(),
                   [&](std::pair<VideoMode, ComPtr<IMFMediaType>>& input) {
                     return input.first == toCheck;
                   });

  if (match == m_windowsVideoModes.end()) {
    return nullptr;
  }
  std::cout << match->first.pixelFormat << ": "
            << (toCheck.pixelFormat == match->first.pixelFormat)
            << " Pixel Format Check Mode\n";
  return match->second;
}

CS_StatusValue UsbCameraImpl::DeviceCmdSetMode(
    std::unique_lock<wpi::mutex>& lock, const Message& msg) {
  VideoMode newMode;
  if (msg.kind == Message::kCmdSetMode) {
    newMode.pixelFormat = msg.data[0];
    newMode.width = msg.data[1];
    newMode.height = msg.data[2];
    newMode.fps = msg.data[3];
  } else if (msg.kind == Message::kCmdSetPixelFormat) {
    newMode = m_mode;
    newMode.pixelFormat = msg.data[0];
  } else if (msg.kind == Message::kCmdSetResolution) {
    newMode = m_mode;
    newMode.width = msg.data[0];
    newMode.height = msg.data[1];
  } else if (msg.kind == Message::kCmdSetFPS) {
    newMode = m_mode;
    newMode.fps = msg.data[0];
  }

  // Check if the device is not connected, if not just apply and leave
  if (!m_properties_cached) {
    m_mode = newMode;
    return CS_OK;
  }


  // If the pixel format or resolution changed, we need to disconnect and
  // reconnect
  if (newMode != m_mode) {
    // First check if the new mode is valid
    auto newModeType = DeviceCheckModeValid(newMode);
    if (!newModeType) {
      return CS_UNSUPPORTED_MODE;
    }

    m_currentMode = std::move(newModeType);
    m_mode = newMode;
    lock.unlock();
    bool wasStreaming = m_streaming;
    if (wasStreaming) DeviceStreamOff();
    if (m_sourceReader) {
      DeviceDisconnect();
      DeviceConnect();
    }
    if (wasStreaming) DeviceStreamOn();
    Notifier::GetInstance().NotifySourceVideoMode(*this, newMode);
    lock.lock();
  }

  return CS_OK;
}

bool UsbCameraImpl::DeviceStreamOn() {
  if (m_streaming) return false;
  if (!m_deviceValid) return false;
  m_streaming = true;
  std::cout << "Calling Read Sample" << std::endl;
  m_sourceReader->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, NULL, NULL,
                             NULL, NULL);
  return true;
}

bool UsbCameraImpl::DeviceStreamOff() {
  m_streaming = false;
  return true;
}

void UsbCameraImpl::DeviceCacheMode() {
  if (!m_sourceReader) return;

  if (!m_currentMode) {

    // First, see if our set mode is valid
    m_currentMode = DeviceCheckModeValid(m_mode);
    if (!m_currentMode) {
      if (FAILED(m_sourceReader->GetCurrentMediaType(
            MF_SOURCE_READER_FIRST_VIDEO_STREAM,
            m_currentMode.GetAddressOf()))) {
      return;
    }
    // Find cached version
    DWORD compare = MF_MEDIATYPE_EQUAL_MAJOR_TYPES |
                    MF_MEDIATYPE_EQUAL_FORMAT_TYPES |
                    MF_MEDIATYPE_EQUAL_FORMAT_DATA;
    auto result = std::find_if(
        m_windowsVideoModes.begin(), m_windowsVideoModes.end(),
        [this, &compare](std::pair<VideoMode, ComPtr<IMFMediaType>>& input) {
          return input.second->IsEqual(m_currentMode.Get(), &compare) == S_OK;
        });

    if (result == m_windowsVideoModes.end()) {
      // Default mode is not supported. Grab first supported image
      auto&& firstSupported = m_windowsVideoModes[0];
      m_currentMode = firstSupported.second;
      std::lock_guard<wpi::mutex> lock(m_mutex);
      m_mode = firstSupported.first;
    } else {
      std::cout << "Setting Current Mode: " << result->first.pixelFormat
                << "\n";
      std::lock_guard<wpi::mutex> lock(m_mutex);
      m_mode = result->first;
    }
    }




  }

  DeviceSetMode();

  std::cout << "Notify change\n";

  Notifier::GetInstance().NotifySourceVideoMode(*this, m_mode);
}

CS_StatusValue UsbCameraImpl::DeviceSetMode() {
  std::cout << "Setting Device Mode\n";

  HRESULT setResult = m_sourceReader->SetCurrentMediaType(
      MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, m_currentMode.Get());

  m_deviceValid = SUCCEEDED(setResult);

  switch (setResult) {
    case S_OK:
      std::cout << "S_OK\n";
      return CS_OK;
      break;
    case MF_E_INVALIDMEDIATYPE:
      std::cout << "InvalidMediaType\n";
      break;
    case MF_E_INVALIDREQUEST:
      std::cout << "Invalid\n";
      break;
    case MF_E_INVALIDSTREAMNUMBER:
      std::cout << "StreamNum\n";
      break;
    case MF_E_TOPO_CODEC_NOT_FOUND:
      std::cout << "Codec\n";
      break;
  }

  return CS_UNSUPPORTED_MODE;
}

void UsbCameraImpl::DeviceCacheVideoModes() {
  if (!m_sourceReader) return;

  std::vector<VideoMode> modes;
  m_windowsVideoModes.clear();

  bool set = false;

  ComPtr<IMFMediaType> nativeType;
  int count = 0;
  while (true) {
    nativeType.Reset();
    auto hr = m_sourceReader->GetNativeMediaType(
        MF_SOURCE_READER_FIRST_VIDEO_STREAM, count, nativeType.GetAddressOf());
    if (FAILED(hr)) {
      break;
    }
    GUID guid = {0};
    nativeType->GetGUID(MF_MT_SUBTYPE, &guid);

    // auto name = cs::GetGUIDNameConstNew(guid);
    auto format = GetFromGUID(guid);
    if (format == VideoMode::kUnknown) {
      count++;
      // Don't put in unknowns
      continue;
    }
    UINT32 width, height;
    ::MFGetAttributeSize(nativeType.Get(), MF_MT_FRAME_SIZE, &width, &height);

    UINT32 num, dom;
    ::MFGetAttributeRatio(nativeType.Get(), MF_MT_FRAME_RATE, &num, &dom);

    // std::cout << "format: " << cs::GetGUIDNameConstNew(guid) << "num: " <<
    // num << "dom: " << dom << "\n";

    int fps = 30;

    if (dom != 0) {
      fps = std::ceil(num / static_cast<double>(dom));
    }

    VideoMode newMode = {format, static_cast<int>(width),
                         static_cast<int>(height), fps};

    modes.emplace_back(newMode);
    m_windowsVideoModes.emplace_back(newMode, nativeType);
    count++;
  }
  {
    std::lock_guard<wpi::mutex> lock(m_mutex);
    m_videoModes.swap(modes);
  }
  Notifier::GetInstance().NotifySource(*this, CS_SOURCE_VIDEOMODES_UPDATED);
}

std::vector<UsbCameraInfo> EnumerateUsbCameras(CS_Status* status) {
  std::vector<UsbCameraInfo> retval;

  // Ensure we are initialized by grabbing the message pump
  // GetMessagePump();

  ComPtr<IMFMediaSource> ppSource;
  std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
  ComPtr<IMFMediaSource> pSource;
  ComPtr<IMFAttributes> pAttributes;
  IMFActivate** ppDevices = nullptr;

  // Create an attribute store to specify the enumeration parameters.
  HRESULT hr = MFCreateAttributes(pAttributes.GetAddressOf(), 1);
  if (FAILED(hr)) {
    goto done;
  }

  // Source type: video capture devices
  hr = pAttributes->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
                            MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);
  if (FAILED(hr)) {
    goto done;
  }

  // Enumerate devices.
  UINT32 count;
  hr = MFEnumDeviceSources(pAttributes.Get(), &ppDevices, &count);
  if (FAILED(hr)) {
    goto done;
  }

  if (count == 0) {
    hr = E_FAIL;
    goto done;
  }

  for (UINT32 i = 0; i < count; i++) {
    UsbCameraInfo info;
    info.dev = i;
    WCHAR buf[512];
    ppDevices[i]->GetString(MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME, buf,
                            sizeof(buf), NULL);
    info.name = utf8_conv.to_bytes(buf);
    ppDevices[i]->GetString(
        MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK, buf,
        sizeof(buf), NULL);
    info.path = utf8_conv.to_bytes(buf);
    retval.emplace_back(std::move(info));
  }

done:
  pAttributes.Reset();

  for (DWORD i = 0; i < count; i++) {
    if (ppDevices[i]) {
      ppDevices[i]->Release();
      ppDevices[i] = nullptr;
    }
  }
  CoTaskMemFree(ppDevices);
  pSource.Reset();
  return retval;
}

CS_Source CreateUsbCameraDev(const wpi::Twine& name, int dev,
                             CS_Status* status) {
  // First check if device exists
  auto devices = cs::EnumerateUsbCameras(status);
  if (devices.size() > dev) {
    return CreateUsbCameraPath(name, devices[dev].path, status);
  }
  auto source = std::make_shared<UsbCameraImpl>(name, dev);
  auto handle = Sources::GetInstance().Allocate(CS_SOURCE_USB, source);
  Notifier::GetInstance().NotifySource(name, handle, CS_SOURCE_CREATED);
  // Start thread after the source created event to ensure other events
  // come after it.
  source->Start();
  return handle;
}

CS_Source CreateUsbCameraPath(const wpi::Twine& name, const wpi::Twine& path,
                              CS_Status* status) {
  auto source = std::make_shared<UsbCameraImpl>(name, path);
  auto handle = Sources::GetInstance().Allocate(CS_SOURCE_USB, source);
  Notifier::GetInstance().NotifySource(name, handle, CS_SOURCE_CREATED);
  // Start thread after the source created event to ensure other events
  // come after it.
  source->Start();
  return handle;
}

std::string GetUsbCameraPath(CS_Source source, CS_Status* status) {
  auto data = Sources::GetInstance().Get(source);
  if (!data || data->kind != CS_SOURCE_USB) {
    *status = CS_INVALID_HANDLE;
    return std::string{};
  }
  return static_cast<UsbCameraImpl&>(*data->source).GetPath();
}

}  // namespace cs
