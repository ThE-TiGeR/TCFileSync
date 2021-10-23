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

#include "TCFile.h"
#include "TCFileName.h"
#include "TCFileSyncFileInfo.h"
#include "TCSystem.h"
#include "TCString.h"

#include "TCNewEnable.h"

namespace tc::file_sync
{

   class ActionBase : public Action
   {
   public:
      ActionBase(std::string action_string)
         : m_action_string(std::move(action_string))
      {
      }

      [[nodiscard]] const std::string& GetActionString() const override
      {
         return m_action_string;
      }

      [[nodiscard]] const std::string& GetErrorMessage() const override
      {
         return m_error;
      }

   protected:
      void SetErrorString(const std::string& error)
      {
         m_error = error;
      }

   private:
      std::string m_error;
      std::string m_action_string;
   };



   class CopyAction final : public ActionBase
   {
   public:
      CopyAction(FileInfo source, FileInfo destination)
         :ActionBase("Copy")
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
         std::string dir_name = file_name::GetPath(m_destination.GetName());
         if (!file::IsDirectory(dir_name) &&
            !file::CreateDirRecursive(dir_name))
         {
            SetErrorString(system::GetLastErrorMessage());
            return false;
         }

         if (file::Exists(m_destination.GetName()))
         {
            file::SetFileAttr(m_destination.GetName(), file::WRITE);
         }

         if (!file::Copy(m_source.GetName(), m_destination.GetName()))
         {
            SetErrorString(system::GetLastErrorMessage() + "\n" + m_source.GetName() + "\n" + m_destination.GetName());
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
         :ActionBase("Create Directory")
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
         if (!file::CreateDirRecursive(m_directory.GetName()))
         {
            SetErrorString(system::GetLastErrorMessage());
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
         :ActionBase("Move")
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
         std::string dir_name = file_name::GetPath(m_destination.GetName());
         if (!file::IsDirectory(dir_name) &&
            !file::CreateDirRecursive(dir_name))
         {
            SetErrorString(system::GetLastErrorMessage());
            return false;
         }

         if (!file::Move(m_source.GetName(), m_destination.GetName()))
         {
            SetErrorString(system::GetLastErrorMessage());
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
         :ActionBase("Delete")
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
         file::SetFileAttr(m_file.GetName(), file::WRITE);
         if (!file::Remove(m_file.GetName()))
         {
            SetErrorString(system::GetLastErrorMessage());
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
