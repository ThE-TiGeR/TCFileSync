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
//  $Id: TCFileSyncSyncronizer.cpp,v 5121974dd9eb 2012/09/17 19:34:55 thomas $
//*******************************************************************************
#include "TCFileSyncSyncronizer.h"

#include "TCFileSyncWindow.h"
#include "TCFileSyncVersion.h"
#include "TCGuiApplication.h"
#include "TCOutput.h"
#include "TCMTLockable.h"
#include "TCString.h"
#include "TCSystem.h"

#include "TCNewEnable.h"

namespace tc
{
    namespace file_sync
    {
        Syncronizer::Syncronizer(const Settings& settings, StatusDisplayerPtr status_displayer)
            :m_action_generator(settings, status_displayer)
            , m_settings(settings)
            , m_status_displayer(status_displayer)
        {
        }

        Syncronizer::~Syncronizer()
        {
        }

        bool Syncronizer::SetupSyncronisationData()
        {
            return m_action_generator.CreateActions();
        }

        bool Syncronizer::SyncDestination()
        {
            const Actions& actions = m_action_generator.GetActions();

            TCINFOS("FileSync", "Syncing " << actions.size() << " destination files ...");

            if (m_status_displayer) m_status_displayer->SetStatusText("Syncing destination");

            // find total bytes to process
            uint64_t total_bytes = 0;
            uint64_t old_percent = 0;
            for (const auto& action : actions)
            {
                total_bytes += action->GetBytesToSync();
            }

            uint64_t bytes_processed = 0;
            for (auto action : actions)
            {
                if (!m_settings.info_mode)
                {
                    if (!action->Do())
                    {
                        TCERRORS("FileSync", "-----------" << action->GetErrorMessage() << "-----------");
                        continue;
                    }
                }

                bytes_processed += action->GetBytesToSync();

                if (m_status_displayer)
                {
                    //TCINFOS("FileSync", (*action_it)->GetActionString() << " " << (*action_it)->GetSource()->GetName() << " -> " << (*action_it)->GetDestination()->GetName());
                    m_status_displayer->SetProgress(0, bytes_processed, total_bytes);
                }
                else
                {
                    uint64_t percent = (100 * bytes_processed) / total_bytes;
                    if (old_percent != percent)
                    {
                        TCINFOS("FileSync", percent << "% done.");
                        old_percent = percent;
                    }
                }
            }

            if (m_status_displayer) m_status_displayer->SetStatusText("");
            TCINFOS("FileSync", "Syncing destination files done.");

            return true;
        }

        const Actions& Syncronizer::GetActions() const
        {
            return m_action_generator.GetActions();
        }
    }
}
