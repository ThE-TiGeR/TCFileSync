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
// Copyright (C) 2003 - 2010 Thomas Goessler. All Rights Reserved. 
//*******************************************************************************
//
// TCSystem is the legal property of its developers.
// Please refer to the COPYRIGHT file distributed with this source distribution.
// 
// This library is free software; you can redistribute it and/or             
// modify it under the terms of the GNU Lesser General Public                
// License as published by the Free Software Foundation; either              
// version 2.1 of the License, or (at your option) any later version.        
//                                                                           
// This library is distributed in the hope that it will be useful,           
// but WITHOUT ANY WARRANTY; without even the implied warranty of            
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         
// Lesser General Public License for more details.                           
//                                                                           
// You should have received a copy of the GNU Lesser General Public          
// License along with this library; if not, write to the Free Software       
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
//*******************************************************************************
//  $Id$
//*******************************************************************************
#include "TCFileSyncActions.h"

#include "TCFile.h"
#include "TCFileName.h"
#include "TCFileSyncFileInfo.h"
#include "TCOutput.h"
#include "TCSystem.h"
#include "TCString.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace FileSync
   {

      class ActionBase: public Action
      {
      public:
         ActionBase(const std::string& action_string)
            :m_error()
            ,m_action_string(action_string)
         {
         }
         virtual const std::string& GetActionString() const
         {
            return m_action_string;
         }

         virtual const std::string& GetErrorMessage() const
         {
            return m_error;
         }

         virtual ~ActionBase() {}

      protected:
         void SetErrorString(const std::string& error)
         {
            m_error = error;
         }

      private:
         std::string m_error;
         std::string m_action_string;
      };


      
      class CopyAction: public ActionBase
      {
      public:
         CopyAction(const FileInfo& source, const FileInfo& destination)
            :ActionBase("Copy")
            ,m_source(source)
            ,m_destination(destination)
         {
         }
         virtual const FileInfo* GetSource() const
         {
            return &m_source;
         }
         virtual const FileInfo* GetDestination() const
         {
            return &m_destination;
         }
         virtual uint64 GetBytesToSync() const
         {
            return m_source.GetFileSize();
         }


         virtual bool Do()
         {
            std::string dir_name = FileName::GetPath(m_destination.GetName());
            if (!File::IsDirectory(dir_name) &&
               !File::CreateDirRecursive(dir_name))
            {
               SetErrorString(System::GetLastErrorMessage());
               return false;
            }

            if (File::Exists(m_destination.GetName()))
            {
               File::SetFileAttr(m_destination.GetName(), File::FILEATTR_WRITE);
            }

            if (!File::Copy(m_source.GetName(), m_destination.GetName()))
            {
               SetErrorString(System::GetLastErrorMessage());
               return false;
            }

            return true;
         }

      private:
         FileInfo m_source;
         FileInfo m_destination;
      };

      class CreateDirectoryAction: public ActionBase
      {
      public:
         CreateDirectoryAction(const FileInfo& directory)
            :ActionBase("Create Directory")
            ,m_directory(directory)
         {
         }
         virtual const FileInfo* GetSource() const
         {
            return &m_directory;
         }
         virtual const FileInfo* GetDestination() const
         {
            return 0;
         }
         virtual uint64 GetBytesToSync() const
         {
            return 1;
         }

         virtual bool Do()
         {
            if (!File::CreateDirRecursive(m_directory.GetName()))
            {
               SetErrorString(System::GetLastErrorMessage());
               return false;
            }

            return true;
         }

      private:
         FileInfo m_directory;
      };

      class MoveAction: public ActionBase
      {
      public:
         MoveAction(const FileInfo& source, const FileInfo& destination)
            :ActionBase("Move")
            ,m_source(source)
            ,m_destination(destination)
         {
         }
         virtual const FileInfo* GetSource() const
         {
            return &m_source;
         }
         virtual const FileInfo* GetDestination() const
         {
            return &m_destination;
         }
         virtual uint64 GetBytesToSync() const
         {
            return 1;
         }

         virtual bool Do()
         {
            std::string dir_name = FileName::GetPath(m_destination.GetName());
            if (!File::IsDirectory(dir_name) &&
               !File::CreateDirRecursive(dir_name))
            {
               SetErrorString(System::GetLastErrorMessage());
               return false;
            }

            if (!File::Move(m_source.GetName(), m_destination.GetName()))
            {
               SetErrorString(System::GetLastErrorMessage());
               return false;
            }

            return true;
         }

      private:
         FileInfo m_source;
         FileInfo m_destination;
      };

      class DeleteAction: public ActionBase
      {
      public:
         DeleteAction(const FileInfo& file)
            :ActionBase("Delete")
            ,m_file(file)
         {
         }
         virtual const FileInfo* GetSource() const
         {
            return &m_file;
         }
         virtual const FileInfo* GetDestination() const
         {
            return 0;
         }
         virtual uint64 GetBytesToSync() const
         {
            return 1;
         }

         virtual bool Do()
         {
            File::SetFileAttr(m_file.GetName(), File::FILEATTR_WRITE);
            if (!File::Remove(m_file.GetName()))
            {
               SetErrorString(System::GetLastErrorMessage());
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
}
