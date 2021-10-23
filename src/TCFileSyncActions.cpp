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

#include "TCFileSyncActions.h"

#include <utility>

#include "TCWFile.h"
#include "TCWFileName.h"
#include "TCFileSyncFileInfo.h"
#include "TCSystem.h"
#include "TCWString.h"

namespace tc::file_sync
{

   class ActionBase : public Action
   {
   public:
      ActionBase(std::wstring action_string)
         : m_action_string(std::move(action_string))
      {
      }

      [[nodiscard]] const std::wstring& GetActionString() const override
      {
         return m_action_string;
      }

      [[nodiscard]] const std::wstring& GetErrorMessage() const override
      {
         return m_error;
      }

   protected:
      void SetErrorString(const std::wstring& error)
      {
         m_error = error;
      }

   private:
      std::wstring m_error;
      std::wstring m_action_string;
   };



   class CopyAction final : public ActionBase
   {
   public:
      CopyAction(FileInfo source, FileInfo destination)
         :ActionBase(L"Copy")
          , m_source(std::move(source))
          , m_destination(std::move(destination))
      {
      }
      const FileInfo* GetSource() const override
      {
         return &m_source;
      }
      const FileInfo* GetDestination() const override
      {
         return &m_destination;
      }
      uint64_t GetBytesToSync() const override
      {
         return m_source.GetFileSize();
      }


      bool Do() override
      {
         std::wstring dir_name = wfile_name::GetPath(m_destination.GetName());
         if (!wfile::IsDirectory(dir_name) &&
            !wfile::CreateDirRecursive(dir_name))
         {
            SetErrorString(wstring::ToString(system::GetLastErrorMessage()));
            return false;
         }

         if (wfile::Exists(m_destination.GetName()))
         {
            wfile::SetFileAttr(m_destination.GetName(), wfile::WRITE);
         }

         if (!wfile::Copy(m_source.GetName(), m_destination.GetName()))
         {
            SetErrorString(wstring::ToString(system::GetLastErrorMessage()) + L"\n" + m_source.GetName() + L"\n" + m_destination.GetName());
            return false;
         }

         return true;
      }

   private:
      FileInfo m_source;
      FileInfo m_destination;
   };

   class CreateDirectoryAction final : public ActionBase
   {
   public:
      CreateDirectoryAction(FileInfo directory)
         :ActionBase(L"Create Directory")
          , m_directory(std::move(directory))
      {
      }
      const FileInfo* GetSource() const override
      {
         return &m_directory;
      }
      const FileInfo* GetDestination() const override
      {
         return nullptr;
      }
      uint64_t GetBytesToSync() const override
      {
         return 1;
      }

      bool Do() override
      {
         if (!wfile::CreateDirRecursive(m_directory.GetName()))
         {
            SetErrorString(wstring::ToString(system::GetLastErrorMessage()));
            return false;
         }

         return true;
      }

   private:
      FileInfo m_directory;
   };

   class MoveAction final : public ActionBase
   {
   public:
      MoveAction(FileInfo source, FileInfo destination)
         :ActionBase(L"Move")
          , m_source(std::move(source))
          , m_destination(std::move(destination))
      {
      }
      const FileInfo* GetSource() const override
      {
         return &m_source;
      }
      const FileInfo* GetDestination() const override
      {
         return &m_destination;
      }
      uint64_t GetBytesToSync() const override
      {
         return 1;
      }

      bool Do() override
      {
         std::wstring dir_name = wfile_name::GetPath(m_destination.GetName());
         if (!wfile::IsDirectory(dir_name) &&
            !wfile::CreateDirRecursive(dir_name))
         {
            SetErrorString(wstring::ToString(system::GetLastErrorMessage()));
            return false;
         }

         if (!wfile::Move(m_source.GetName(), m_destination.GetName()))
         {
            SetErrorString(wstring::ToString(system::GetLastErrorMessage()));
            return false;
         }

         return true;
      }

   private:
      FileInfo m_source;
      FileInfo m_destination;
   };

   class DeleteAction final : public ActionBase
   {
   public:
      DeleteAction(FileInfo file)
         :ActionBase(L"Delete")
          , m_file(std::move(file))
      {
      }
      const FileInfo* GetSource() const override
      {
         return &m_file;
      }
      const FileInfo* GetDestination() const override
      {
         return nullptr;
      }
      uint64_t GetBytesToSync() const override
      {
         return 1;
      }

      bool Do() override
      {
         wfile::SetFileAttr(m_file.GetName(), wfile::WRITE);
         if (!wfile::Remove(m_file.GetName()))
         {
            SetErrorString(wstring::ToString(system::GetLastErrorMessage()));
            return false;
         }

         return true;
      }

   private:
      FileInfo m_file;
   };

   ActionPtr CreateCopyAction(const FileInfo& source, const FileInfo& destination)
   {
      return ActionPtr(new CopyAction(source, destination));
   }
   ActionPtr CreateCreateDirectoryAction(const FileInfo& directory)
   {
      return ActionPtr(new CreateDirectoryAction(directory));
   }
   ActionPtr CreateMoveAction(const FileInfo& source, const FileInfo& destination)
   {
      return ActionPtr(new MoveAction(source, destination));
   }
   ActionPtr CreateDeleteAction(const FileInfo& file)
   {
      return ActionPtr(new DeleteAction(file));
   }

}
