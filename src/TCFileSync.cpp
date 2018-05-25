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
// Copyright (C) 2003 - 2018 Thomas Goessler. All Rights Reserved. 
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
//  $Id: TCFileSync.cpp,v 5121974dd9eb 2012/09/17 19:34:55 thomas $
//*******************************************************************************
// CONFIX:EXENAME("TCFileSync")

#include "TCFileSyncSyncronizer.h"
#include "TCFileSyncWindow.h"
#include "TCFileSyncVersion.h"
#include "TCGuiApplication.h"
#include "TCOutput.h"
#include "TCMTLockable.h"
#include "TCString.h"
#include "TCFileName.h"
#include "TCSystem.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace file_sync
   {
      static StreamPtr s_out;

      class MTTraceTarget: public output::PrintTarget,
         public multi_threading::ObjectLevelLockable<MTTraceTarget>
      {
      public:
         MTTraceTarget()
         {
         }

         virtual void Print(const char* text)
         {
            Locker lock(this);
            s_out << text << endl << flush;
         }
      private:
         friend class multi_threading::LockerPtr< MTTraceTarget* >;
      };

      class GuiTraceTarget: public output::PrintTarget
      {
      public:
         GuiTraceTarget(FX::FXList* list)
            :m_list(list)
         {
         }

         virtual void Print(const char* text)
         {
            m_list->appendItem(text);
            m_list->update();

            while (m_list->getNumItems() > 50)
            {
               m_list->removeItem(0);
               m_list->update();
            }

            m_list->makeItemVisible(m_list->getNumItems()-1);
            m_list->update();
            FX::FXApp::instance()->runWhileEvents();
         }
      private:
         FX::FXList* m_list;
      };

      class GuiApplication: public gui::Application
      {
      public:
         GuiApplication()
            :m_settings(),
            m_start_gui(true),
            m_window(0)
         {
            s_out = factory::CreateStdOutStream();

            output::PrintTargetPtr trace_target(new MTTraceTarget);
            output::SetErrorTarget(trace_target);
            output::SetWarningTarget(trace_target);
            output::SetInfoTarget(trace_target);
            output::SetTraceTarget(trace_target);

            s_out << PROGRAM_NAME ": Version " PROGRAM_VERSION_STR << endl
               << "            This product includes: " << TCPRODUCT_STR "-" TCVERSION_STR
               << endl << endl;

            m_settings.info_mode     = false;
            m_settings.calc_checksum = false;
            m_settings.backup_folder = "_Older";
            m_settings.num_backups   = 5;
        }

         ~GuiApplication()
         {
            output::SetErrorTarget(output::PrintTargetPtr());
            output::SetWarningTarget(output::PrintTargetPtr());
            output::SetInfoTarget(output::PrintTargetPtr());
            output::SetTraceTarget(output::PrintTargetPtr());

            s_out = StreamPtr();
         }

         virtual bool Run()
         {
            if (m_start_gui)
            {
               output::PrintTargetPtr trace_target(new GuiTraceTarget(m_window->GetInfoList()));
               output::SetErrorTarget(trace_target);
               output::SetWarningTarget(trace_target);
               output::SetInfoTarget(trace_target);
               output::SetTraceTarget(trace_target);

               return gui::Application::Run();
            }

            if (m_window)
            {
               m_window->hide();
            }


            Syncronizer syncronicer(m_settings, StatusDisplayerPtr());
            if (!syncronicer.SetupSyncronisationData())
            {
               TCERRORS("FileSync", "Failed setting up synchronization data");
               return false;
            }
            if (!syncronicer.SyncDestination())
            {
               TCERRORS("FileSync", "Failed syncing directories");
               return false;
            }

            return true;
         }

      protected:
         virtual bool ProcessArguments(const std::vector< std::string >& argv)
         {
            for (std::vector<std::string>::const_iterator it=argv.begin(); it!=argv.end(); ++it)
            {
               if (*it == "--source" || *it == "-s")
               {
                  ++it;
                  m_settings.source = file_name::Simplify(*it);
               }
               else if (*it == "--destination" || *it == "-d")
               {
                  ++it;
                  m_settings.destination = file_name::Simplify(*it);
               }
               else if (*it == "--backup_folder" || *it == "-b")
               {
                  ++it;
                  m_settings.backup_folder = file_name::Simplify(*it);
               }
               else if (*it == "--num_backups" || *it == "-n")
               {
                  ++it;
                  m_settings.num_backups = string::ToUint32(*it);
               }
               else if (*it == "--skipp")
               {
                  ++it;
                  m_settings.folders_to_skipp.insert(*it);
               }
               else if (*it == "--skipp_ext")
               {
                  ++it;
                  m_settings.extensions_to_skipp.insert(*it);
               }
               else if (*it == "--ext")
               {
                  ++it;
                  m_settings.extensions_to_search_for.insert(*it);
               }
               else if (*it == "--info_only" || *it == "-i")
               {
                  m_settings.info_mode = true;
               }
               else if (*it == "--calc_checksum" || *it == "-c")
               {
                  m_settings.calc_checksum = true;
               }
               else if (*it == "--no_gui")
               {
                  m_start_gui = false;
               }
               else
               {
                  DisplayUsage();
                  return false;
               }
            }

            if ((m_settings.source.empty() || m_settings.destination.empty()) && !m_start_gui)
            {
               DisplayUsage();
               return false;
            }

            return true;

         };

         virtual std::string GetUsage()
         {
            return 
            "Usage: -s or --source        for source directory\n"
            "       -d or --destination   for destination directory\n"
            "       -b or --backup_folder folder into which to make backups of removed or modified files\n"
            "                             default = _Older\n"
            "       -n or --num_backups   number of backups to keep for one file\n"
            "                             default = 5\n"
            "       --ext                 use only files with specified extension during synchronization, can be set more than once\n"
            "       --skip                folder name to skip during synchronization, can be set more than once\n"
            "       --skip_ext            skip filenames with specified extension during synchronization, can be set more than once\n";
         }

         virtual FX::FXMainWindow* CreateMainWindow()
         {
            m_window = new Window(this, m_settings);
            return m_window;
         }

      private:
         Settings m_settings;
         bool m_start_gui;
         Window* m_window;
      };

   }
}

int main(int narg, char** argv)
{
   {
      std::auto_ptr<tc::file_sync::GuiApplication> app(new tc::file_sync::GuiApplication);
      if (!app->Init(narg, argv, PROGRAM_NAME, PROGRAM_VERSION_STR, PROGRAM_COMPANY))
      {
         return 1;
      }

      if (!app->Run())
      {
         return 2;
      }
   }

   return 0;
}

