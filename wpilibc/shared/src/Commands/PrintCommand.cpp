/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2011-2017. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Commands/PrintCommand.h"

#include "llvm/raw_ostream.h"

frc::PrintCommand::PrintCommand(const std::string& message)
    : InstantCommand("Print \"" + message + "\"") {
  m_message = message;
}

void frc::PrintCommand::Initialize() { llvm::outs() << m_message << "\n"; }
