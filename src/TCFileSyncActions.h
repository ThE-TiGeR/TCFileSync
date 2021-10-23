//*******************************************************************************
//
// *******   ***   ***               *
//    *     *     *                  *
//    *    *      *                *****
//    *    *       ***  *   *   **   *    **    ***
//    *    *          *  * *   *     *   ****  * * *
//    *     *         *   *      *   * * *     * * *
//    *      ***   ***    *     **   **   **   *   *
//                        *
//*******************************************************************************
// see http://sourceforge.net/projects/tcsystem/ for details.
// Copyright (C) 2003 - 2021 Thomas Goessler. All Rights Reserved. 
//*******************************************************************************
#pragma once

#include "TCFileSyncFileInfo.h"
#include "TCSharedPtr.h"

#include <string>
#include <vector> 

namespace tc::file_sync
{
   class Action
   {
   public:
      [[nodiscard]] virtual const std::wstring& GetActionString() const = 0;
      [[nodiscard]] virtual const FileInfo* GetSource() const = 0;
      [[nodiscard]] virtual const FileInfo* GetDestination() const = 0;
      [[nodiscard]] virtual uint64_t GetBytesToSync() const = 0;

      [[nodiscard]] virtual const std::wstring& GetErrorMessage() const = 0;

      virtual bool Do() = 0;

      virtual ~Action() = default;
   };

   typedef SharedPtr<Action> ActionPtr;
   typedef std::vector<ActionPtr> Actions;

   [[nodiscard]] ActionPtr CreateCopyAction(const FileInfo& source, const FileInfo& destination);
   [[nodiscard]] ActionPtr CreateCreateDirectoryAction(const FileInfo& directory);
   [[nodiscard]] ActionPtr CreateMoveAction(const FileInfo& source, const FileInfo& destination);
   [[nodiscard]] ActionPtr CreateDeleteAction(const FileInfo& file);
}
