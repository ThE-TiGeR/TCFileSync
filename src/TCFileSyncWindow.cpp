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
#include "TCFileSyncWindow.h"

#include "TCFileSyncActionWindow.h"
#include "TCFileSyncVersion.h"
#include "TCFileSyncSyncronizer.h"
#include "TCFileSyncOptionsWindow.h"
#include "TCGuiImageHandler.h"
#include "TCGuiButton.h"
#include "TCGuiLabel.h"
#include "TCGuiMessageBox.h"
#include "TCGuiMenu.h"
#include "TCOutput.h"
#include "TCString.h"
#include "TCTime.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace file_sync
   {
      FXDEFMAP(Window) WindowMap[] =
      {
         //________Message_Type_________ID_____________________Message_Handler_______
         FXMAPFUNC(FX::SEL_CLOSE,         0,                       Window::OnCmdQuit),
         FXMAPFUNC(FX::SEL_COMMAND,       Window::ID_QUIT,         Window::OnCmdQuit),
         FXMAPFUNC(FX::SEL_COMMAND,       Window::ID_ABOUT,        Window::OnCmdAbout),
         FXMAPFUNC(FX::SEL_COMMAND,       Window::ID_OPTIONS,      Window::OnCmdOptions),

         FXMAPFUNC(FX::SEL_COMMAND,       Window::ID_SOURCE_DIR,     Window::OnCmdSourceDir),
         FXMAPFUNC(FX::SEL_COMMAND,       Window::ID_SEL_SOURCE_DIR, Window::OnCmdSelSourceDir),
         FXMAPFUNC(FX::SEL_COMMAND,       Window::ID_TARGET_DIR,     Window::OnCmdTargetDir),
         FXMAPFUNC(FX::SEL_COMMAND,       Window::ID_SEL_TARGET_DIR, Window::OnCmdSelTargetDir),
         FXMAPFUNC(FX::SEL_COMMAND,       Window::ID_SYNC,           Window::OnCmdSync),

         FXMAPFUNC(FX::SEL_UPDATE,       Window::ID_SOURCE_DIR,     Window::OnUpdSourceDir),
         FXMAPFUNC(FX::SEL_UPDATE,       Window::ID_SEL_SOURCE_DIR, Window::OnUpdSelSourceDir),
         FXMAPFUNC(FX::SEL_UPDATE,       Window::ID_TARGET_DIR,     Window::OnUpdTargetDir),
         FXMAPFUNC(FX::SEL_UPDATE,       Window::ID_SEL_TARGET_DIR, Window::OnUpdSelTargetDir),
         FXMAPFUNC(FX::SEL_UPDATE,       Window::ID_SYNC,           Window::OnUpdSync),
      };

      TCGUIBASE_IMPLEMENT(Window, FX::FXMainWindow, WindowMap, ARRAYNUMBER(WindowMap))

      Window::Window(gui::Application* appl, const Settings& settings)
         :FX::FXMainWindow(FX::FXApp::instance(), PROGRAM_NAME, 
         gui::ImageHandler::GetInstance()->GetIcon("tc_im_chat-window"),
         gui::ImageHandler::GetInstance()->GetIcon("tc_im_chat-window"),
         FX::DECOR_ALL, 500, 50, 400, 0, 0, 0),
         m_appl(appl),
         m_settings(settings),
         m_file_menu(0),
         m_help_menu(0),
         m_info_list(0),
         m_progress_label(0),
         m_progress_bar(0)
      {
         // Make menu bar
         FX::FXToolBarShell* drag_shell=new FX::FXToolBarShell(this, 0,0,0,0,0,0,0);
         FX::FXMenuBar* menu_bar=new FX::FXMenuBar(this, drag_shell,FX::FRAME_RAISED|FX::LAYOUT_SIDE_TOP|FX::LAYOUT_FILL_X);
         new FX::FXToolBarGrip(menu_bar,this,FX::FXToolBar::ID_TOOLBARGRIP,FX::TOOLBARGRIP_DOUBLE);

         // file menu
         m_file_menu = new FX::FXMenuPane(this);
         new gui::MenuTitle(menu_bar, "TC_FILE_SYNC_FILE", 0, m_file_menu);
         new gui::MenuCommand(m_file_menu, "TC_FILE_SYNC_OPTIONS", 0, this, ID_OPTIONS);
         new gui::MenuCommand(m_file_menu, "TC_FILE_SYNC_EXIT", 0, this, ID_QUIT);

         // Help menu
         m_help_menu = new FX::FXMenuPane(this);
         new gui::MenuTitle(menu_bar, "TC_FILE_SYNC_HELP", 0, m_help_menu);
         new gui::MenuCommand(m_help_menu, "TC_FILE_SYNC_ABOUT", 0, this, ID_ABOUT);

         FX::FXMatrix* matrix = new FX::FXMatrix(this, 3, FX::MATRIX_BY_COLUMNS|FX::LAYOUT_FILL_X);
         
         new gui::Label(matrix, "TC_FILE_SYNC_SOURCE");
         new FX::FXTextField(matrix, 20, this, ID_SOURCE_DIR, FX::TEXTFIELD_NORMAL|FX::LAYOUT_FILL_X|FX::LAYOUT_FILL_COLUMN);
         new gui::Button(matrix, "...", 0, this, ID_SEL_SOURCE_DIR);

         new gui::Label(matrix, "TC_FILE_SYNC_TARGET");
         new FX::FXTextField(matrix, 20, this, ID_TARGET_DIR, FX::TEXTFIELD_NORMAL|FX::LAYOUT_FILL_X|FX::LAYOUT_FILL_COLUMN);
         new gui::Button(matrix, "...", 0, this, ID_SEL_TARGET_DIR);

         FX::FXVerticalFrame* frame = new FX::FXVerticalFrame(this, FX::LAYOUT_FILL);
         new gui::Button(frame, "TC_FILE_SYNC_SYNCRONIZE", 0, this, ID_SYNC, FX::BUTTON_NORMAL|FX::LAYOUT_RIGHT);
         m_progress_bar = new FX::FXProgressBar(frame, 0, 0,FX::PROGRESSBAR_PERCENTAGE|FX::LAYOUT_BOTTOM|FX::LAYOUT_FILL_X);
         m_progress_label = new FX::FXLabel(frame, "", 0 , FX::LABEL_NORMAL|FX::LAYOUT_BOTTOM|FX::LAYOUT_FILL_X);

         frame = new FX::FXVerticalFrame(frame, FX::LAYOUT_FILL|FX::FRAME_SUNKEN|FX::FRAME_GROOVE);
         m_info_list = new FX::FXList(frame, 0, 0, FX::LIST_NORMAL|FX::LAYOUT_FILL|FX::FRAME_SUNKEN);
      }

      Window::~Window()
      {
         delete m_help_menu;
         delete m_file_menu;
      }

      void Window::create()
      {
         FX::FXMainWindow::create();
         m_progress_label->hide();
         m_progress_bar->hide();
         show(FX::PLACEMENT_SCREEN);
      }

      long Window::OnCmdQuit(FX::FXObject*, FX::FXSelector, void*)
      {
         static_cast<gui::Application*>(Application::GetInstance())->Exit(0);
         return 0;
      }

      long Window::OnCmdAbout(FX::FXObject*, FX::FXSelector, void*)
      {
         std::string mess;

         mess = mess + Application::GetInstance()->GetProgrammName();
         mess = mess + "\n\nVersion " + Application::GetInstance()->GetProgrammVersion(); 
         mess = mess + "\n\n" + TCCOPYRIGHT;
         mess = mess + "\nHome Page: " + TCHOMPAGE;
         mess = mess + "\n\nThis product includes: fox-"
            + string::ToString(FOX_MAJOR) + "." 
            + string::ToString(FOX_MINOR) + "." 
            + string::ToString(FOX_LEVEL);
         mess = mess + ", " + TCPRODUCT_STR + "-" TCVERSION_STR;
         gui::MessageBox::Information(this, "About", mess);

         return 0;
      }

      long Window::OnCmdOptions(FX::FXObject*, FX::FXSelector, void*)
      {
         OptionsWindow dialog(this, m_settings);
         if (dialog.execute())
         {
            m_settings = dialog.GetSettings();
         }

         return 0;
      }

      long Window::OnCmdSourceDir(FX::FXObject*, FX::FXSelector, void* ptr)
      {
         const char* text = static_cast<const char*>(ptr);
         m_settings.source = text;
         return 0;
      }

      long Window::OnCmdSelSourceDir(FX::FXObject*, FX::FXSelector, void*)
      {
         FX::FXDirDialog dir_dialog(this, "Select source directory");
         dir_dialog.setDirectory(m_settings.destination.c_str());
         if (dir_dialog.execute())
         {
            m_settings.source = dir_dialog.getDirectory().text();
         }

         return 0;
      }

      long Window::OnCmdTargetDir(FX::FXObject*, FX::FXSelector, void* ptr)
      {
         const char* text = static_cast<const char*>(ptr);
         m_settings.destination = text;
         return 0;
      }

      long Window::OnCmdSelTargetDir(FX::FXObject*, FX::FXSelector, void*)
      {
         FX::FXDirDialog dir_dialog(this, "Select destination directory");
         dir_dialog.setDirectory(m_settings.destination.c_str());
         if (dir_dialog.execute())
         {
            m_settings.destination = dir_dialog.getDirectory().text();
         }

         return 0;
      }

      class WindowProgressBarStatus: public StatusDisplayer
      {
      public:
         WindowProgressBarStatus(FX::FXLabel* label, FX::FXProgressBar* progress_bar)
            :m_progress_label(label)
            ,m_progress_bar(progress_bar)
         {
         }

         virtual void SetStatusText(const std::string& status)
         {
            if (status.empty())
            {
               m_progress_label->hide();
               m_progress_bar->hide();
            }
            else
            {
               m_progress_label->show();
               m_progress_bar->show();
            }
            m_start_time = Time::NowMonotonic();

            m_status_text = status;
            m_last_time_update_percent = 0;
            m_progress_label->setText(status.c_str());
            m_progress_bar->setProgress(0);
            FX::FXApp::instance()->runWhileEvents();
         }

         virtual void SetProgress(uint64_t start, uint64_t current, uint64_t end)
         {
            double percent = current / double(end -start);
            if (percent > m_last_time_update_percent + 0.01)
            {
               Time current_time = Time::SinceMonotonic(m_start_time);
               Time remaining = Time::FromNanoSeconds(uint64_t(current_time.ToNanoSeconds() / percent * (1.0 - percent)));
               std::string status_text = m_status_text + " remaining " + string::ToString(remaining.ToSeconds()) + " seconds";
               m_progress_label->setText(status_text.c_str());
               m_last_time_update_percent = percent;
            }

            m_progress_bar->setTotal(FX::FXuint(end - start));
            m_progress_bar->setProgress(FX::FXuint(current));
            FX::FXApp::instance()->runWhileEvents();
         }

      private:
         FX::FXLabel* m_progress_label;
         FX::FXProgressBar* m_progress_bar;
         double m_last_time_update_percent;
         std::string m_status_text;
         Time m_start_time;
      };

      long Window::OnCmdSync(FX::FXObject*, FX::FXSelector, void*)
      {
         Syncronizer syncronicer(m_settings, StatusDisplayerPtr(new WindowProgressBarStatus(m_progress_label, m_progress_bar)));
         if (!syncronicer.SetupSyncronisationData())
         {
            TCERRORS("FileSync", "Failed setting up synchronization data");
            return 0;
         }

//          ActionWindow* aw = new ActionWindow(this, syncronicer.GetActions());
//          aw->create();

         if (!syncronicer.SyncDestination())
         {
            TCERRORS("FileSync", "Failed syncing directories");
            return 0;
         }

         return 0;
      }

      long Window::OnUpdSourceDir(FX::FXObject* obj, FX::FXSelector, void*)
      {
         obj->handle(this, MKUINT(FX::FXWindow::ID_SETVALUE, FX::SEL_COMMAND),
            (void*)m_settings.source.c_str());
         return 0;
      }

      long Window::OnUpdSelSourceDir(FX::FXObject*, FX::FXSelector, void*)
      {
         return 0;
      }

      long Window::OnUpdTargetDir(FX::FXObject* obj, FX::FXSelector, void*)
      {
         obj->handle(this, MKUINT(FX::FXWindow::ID_SETVALUE, FX::SEL_COMMAND),
            (void*)m_settings.destination.c_str());
         return 0;
      }

      long Window::OnUpdSelTargetDir(FX::FXObject*, FX::FXSelector, void*)
      {
         return 0;
      }

      long Window::OnUpdSync(FX::FXObject* obj, FX::FXSelector, void*)
      {
         if (m_settings.source.empty() || m_settings.destination.empty() ||
             !file::IsDirectory(m_settings.source) ||
             !file::IsDirectory(m_settings.destination))
         {
            obj->handle(this, MKUINT(FX::FXWindow::ID_DISABLE, FX::SEL_COMMAND), 0);
         }
         else
         {
            obj->handle(this, MKUINT(FX::FXWindow::ID_ENABLE, FX::SEL_COMMAND), 0);
         }
         return 0;
      }

   }

}

