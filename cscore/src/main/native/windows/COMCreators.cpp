/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <mfapi.h>
#include <mfidl.h>
#include <shlwapi.h>
#include <windowsx.h>

#include <iostream>

#include <Windows.h>

#include "UsbCameraImpl.h"

// https://github.com/opencv/opencv/blob/master/modules/videoio/src/cap_msmf.cpp

#include <mfidl.h>
#include <mfapi.h>
#include <Dbt.h>
#include <ks.h>
#include <ksmedia.h>
#include <mfreadwrite.h>

#include "COMCreators.h"
#include "ComPtr.h"

#pragma comment(lib, "Mfplat.lib")
#pragma comment(lib, "Mf.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "Shlwapi.lib")

namespace cs {

// Source callback used by the source reader.
// COM object, so it needs a to ref count itself.
class SourceReaderCB : public IMFSourceReaderCallback {
 public:
  explicit SourceReaderCB(cs::UsbCameraImpl* source)
      : m_nRefCount(1), m_source(source) {}

  // IUnknown methods
  STDMETHODIMP QueryInterface(REFIID iid, void** ppv) {
    static const QITAB qit[] = {
        QITABENT(SourceReaderCB, IMFSourceReaderCallback),
        {0},
    };
    return QISearch(this, qit, iid, ppv);
  }
  STDMETHODIMP_(ULONG) AddRef() { return InterlockedIncrement(&m_nRefCount); }
  STDMETHODIMP_(ULONG) Release() {
    ULONG uCount = InterlockedDecrement(&m_nRefCount);
    if (uCount == 0) {
      delete this;
    }
    return uCount;
  }

  // IMFSourceReaderCallback methods
  STDMETHODIMP OnReadSample(HRESULT hrStatus, DWORD dwStreamIndex,
                            DWORD dwStreamFlags, LONGLONG llTimestamp,
                            IMFSample* pSample);

  STDMETHODIMP OnEvent(DWORD, IMFMediaEvent*) { return S_OK; }

  STDMETHODIMP OnFlush(DWORD) { return S_OK; }

 private:
  // Destructor is private. Caller should call Release.
  virtual ~SourceReaderCB() {}

  void NotifyError(HRESULT hr) { wprintf(L"Source Reader error: 0x%X\n", hr); }

 private:
  ULONG m_nRefCount;  // Reference count.
  UsbCameraImpl* m_source;
};

HRESULT SourceReaderCB::OnReadSample(HRESULT hrStatus, DWORD dwStreamIndex,
                                     DWORD dwStreamFlags, LONGLONG llTimestamp,
                                     IMFSample* pSample  // Can be NULL
) {
  if (SUCCEEDED(hrStatus)) {
    if (pSample) {
      // Prcoess sample
      m_source->ProcessFrame(pSample);
      pSample->Release();
    }
  } else {
    // Streaming error.
    NotifyError(hrStatus);
  }
  // Trigger asking for a new frame.
  // This is piped through the message pump for concurrency reasons
  m_source->PostRequestNewFrame();
  return S_OK;
}

// Create a Source Reader COM Smart Object
ComPtr<IMFSourceReaderCallback> CreateSourceReaderCB(UsbCameraImpl* source) {
  IMFSourceReaderCallback* ptr = new SourceReaderCB(source);
  ComPtr<IMFSourceReaderCallback> sourceReaderCB;
  sourceReaderCB.Attach(ptr);
  return sourceReaderCB;
}

ComPtr<IMFMediaSource> CreateVideoCaptureDevice(LPCWSTR pszSymbolicLink) {
  ComPtr<IMFAttributes> pAttributes;
  ComPtr<IMFMediaSource> pSource;

  HRESULT hr = MFCreateAttributes(pAttributes.GetAddressOf(), 2);

  // Set the device type to video.
  if (SUCCEEDED(hr)) {
    hr = pAttributes->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
                              MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);
  }

  // Set the symbolic link.
  if (SUCCEEDED(hr)) {
    hr = pAttributes->SetString(
        MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK,
        pszSymbolicLink);
  }

  if (SUCCEEDED(hr)) {
    hr = MFCreateDeviceSource(pAttributes.Get(), pSource.GetAddressOf());
  }

  // No need to check last HR, as the source would be null anyway.
  return pSource;
}

ComPtr<IMFSourceReader> CreateSourceReader(IMFMediaSource* mediaSource,
                                           IMFSourceReaderCallback* callback) {
  HRESULT hr = S_OK;
  ComPtr<IMFAttributes> pAttributes;
  ComPtr<IMFSourceReader> sourceReader;

  hr = MFCreateAttributes(pAttributes.GetAddressOf(), 1);
  if (FAILED(hr)) {
    return nullptr;
  }

  hr = pAttributes->SetUnknown(MF_SOURCE_READER_ASYNC_CALLBACK, callback);
  if (FAILED(hr)) {
    return nullptr;
  }

  hr = pAttributes->SetUINT32(
      MF_SOURCE_READER_DISCONNECT_MEDIASOURCE_ON_SHUTDOWN, TRUE);
  if (FAILED(hr)) {
    return nullptr;
  }

  MFCreateSourceReaderFromMediaSource(mediaSource, pAttributes.Get(),
                                      sourceReader.GetAddressOf());

  // No need to check last HR, as the sourceReader would be null anyway.
  return sourceReader;
}

}  // namespace cs
