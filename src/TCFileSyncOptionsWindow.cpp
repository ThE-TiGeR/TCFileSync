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
//  $Id: TCFileSyncOptionsWindow.cpp,v 377861d4ecde 2012/04/24 22:25:51 thomas $
//*******************************************************************************

#include "TCFileSyncOptionsWindow.h"

#include "TCGuiLabel.h"
#include "TCMLSHandler.h"

#include "TCNewEnable.h"

namespace tc
{
    namespace file_sync
    {

        FXDEFMAP(OptionsWindow) OptionsWindowMap[] =
        {
           FXMAPFUNC(FX::SEL_COMMAND, OptionsWindow::ID_PREVIEW, OptionsWindow::OnCmdPreview),
           FXMAPFUNC(FX::SEL_UPDATE,  OptionsWindow::ID_PREVIEW, OptionsWindow::OnUpdPreview),
           FXMAPFUNC(FX::SEL_COMMAND, OptionsWindow::ID_CALC_CHECKSUM, OptionsWindow::OnCmdCalcChecksum),
           FXMAPFUNC(FX::SEL_UPDATE,  OptionsWindow::ID_CALC_CHECKSUM, OptionsWindow::OnUpdCalcChecksum),
           FXMAPFUNC(FX::SEL_COMMAND, OptionsWindow::ID_NUM_BACKUPS, OptionsWindow::OnCmdNumBackups),
           FXMAPFUNC(FX::SEL_UPDATE,  OptionsWindow::ID_NUM_BACKUPS, OptionsWindow::OnUpdNumBackups),
           FXMAPFUNC(FX::SEL_COMMAND, OptionsWindow::ID_BACKUP_FOLDER, OptionsWindow::OnCmdBackupFolder),
           FXMAPFUNC(FX::SEL_UPDATE,  OptionsWindow::ID_BACKUP_FOLDER, OptionsWindow::OnUpdBackupFolder)
        };

        TCGUIBASE_IMPLEMENT(OptionsWindow, gui::ActionBox, OptionsWindowMap, ARRAYNUMBER(OptionsWindowMap))

            OptionsWindow::OptionsWindow(FX::FXWindow* owner, const Settings& settings)
            :gui::ActionBox(owner, "Options"),
            m_settings(settings)
        {
            closeOnExecute = true;
            FX::FXComposite* frame = GetPacker();
            FX::FXVerticalFrame* fv = new FX::FXVerticalFrame(frame, FX::LAYOUT_FILL_X | FX::LAYOUT_FILL_Y);

            FX::FXHorizontalFrame* fh = new FX::FXHorizontalFrame(fv, FX::LAYOUT_FILL_X);
            new FX::FXCheckButton(fh, mls::Handler::GetText("TC_FILE_SYNC_PREVIEW").c_str(), this, ID_PREVIEW, FX::CHECKBUTTON_NORMAL | FX::LAYOUT_RIGHT);
            new FX::FXCheckButton(fh, mls::Handler::GetText("Calculate Checksum").c_str(), this, ID_CALC_CHECKSUM, FX::CHECKBUTTON_NORMAL | FX::LAYOUT_RIGHT);

            fh = new FX::FXHorizontalFrame(fv, FX::LAYOUT_FILL_X);
            new gui::Label(fh, "TC_FILE_SYNC_NUM_BACKUPS");
            new FX::FXTextField(fh, 3, this, ID_NUM_BACKUPS, FX::TEXTFIELD_NORMAL | FX::LAYOUT_FILL_X);
            new gui::Label(fh, "TC_FILE_SYNC_BACKUP_FOLDER");
            new FX::FXTextField(fh, 10, this, ID_BACKUP_FOLDER, FX::TEXTFIELD_NORMAL | FX::LAYOUT_FILL_X);
        }

        long OptionsWindow::OnCmdPreview(FX::FXObject *obj, FX::FXSelector /*sel*/, void* /*ptr*/)
        {
            FX::FXint val = 0;
            obj->handle(this, MKUINT(FX::FXWindow::ID_GETINTVALUE, FX::SEL_COMMAND), &val);

            m_settings.info_mode = val ? true : false;
            return 0;
        }

        long OptionsWindow::OnUpdPreview(FX::FXObject *obj, FX::FXSelector /*sel*/, void* /*ptr*/)
        {
            FX::FXint val = m_settings.info_mode ? 1 : 0;
            obj->handle(this, MKUINT(FX::FXWindow::ID_SETINTVALUE, FX::SEL_COMMAND), &val);

            return 0;
        }

        long OptionsWindow::OnCmdCalcChecksum(FX::FXObject *obj, FX::FXSelector /*sel*/, void* /*ptr*/)
        {
            FX::FXint val = 0;
            obj->handle(this, MKUINT(FX::FXWindow::ID_GETINTVALUE, FX::SEL_COMMAND), &val);

            m_settings.calc_checksum = val ? true : false;
            return 0;
        }

        long OptionsWindow::OnUpdCalcChecksum(FX::FXObject *obj, FX::FXSelector /*sel*/, void* /*ptr*/)
        {
            FX::FXint val = m_settings.calc_checksum ? 1 : 0;
            obj->handle(this, MKUINT(FX::FXWindow::ID_SETINTVALUE, FX::SEL_COMMAND), &val);

            return 0;
        }

        long OptionsWindow::OnCmdNumBackups(FX::FXObject *obj, FX::FXSelector /*sel*/, void* /*ptr*/)
        {
            FX::FXint val = 0;
            obj->handle(this, MKUINT(FX::FXWindow::ID_GETINTVALUE, FX::SEL_COMMAND), &val);

            m_settings.info_mode = val ? true : false;
            return 0;
        }

        long OptionsWindow::OnUpdNumBackups(FX::FXObject *obj, FX::FXSelector /*sel*/, void* /*ptr*/)
        {
            FX::FXint val = m_settings.num_backups;
            obj->handle(this, MKUINT(FX::FXWindow::ID_SETINTVALUE, FX::SEL_COMMAND), &val);

            return 0;
        }

        long OptionsWindow::OnCmdBackupFolder(FX::FXObject *obj, FX::FXSelector /*sel*/, void* /*ptr*/)
        {
            FX::FXString val;
            obj->handle(this, MKUINT(FX::FXWindow::ID_GETINTVALUE, FX::SEL_COMMAND), &val);

            m_settings.backup_folder = val.text();
            return 0;
        }

        long OptionsWindow::OnUpdBackupFolder(FX::FXObject *obj, FX::FXSelector /*sel*/, void* /*ptr*/)
        {
            FX::FXString val = m_settings.backup_folder.c_str();
            obj->handle(this, MKUINT(FX::FXWindow::ID_SETSTRINGVALUE, FX::SEL_COMMAND), &val);

            return 0;
        }
    }
}