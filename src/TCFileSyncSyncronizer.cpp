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
      Syncronizer::Syncronizer(const Settings& settings)
         :m_action_generator(settings),
         m_settings(settings)
      {
      }

      Syncronizer::~Syncronizer()
      {
      }

      bool Syncronizer::SetupSyncronisationData()
      {
         return m_action_generator.CreateActions();
      }

      bool Syncronizer::SyncDestination(StatusDisplayerPtr status_displayer)
      {
         const Actions& actions = m_action_generator.GetActions();

         TCINFOS("FileSync", "Syncing " << actions.size() << " destination files ...");

         if (status_displayer) status_displayer->SetStatusText("Syncing destination");

         // find total bytes to process
         uint64 total_bytes = 0;
         uint64 old_percent = 0;
         for (Actions::const_iterator action_it=actions.begin(); action_it!=actions.end(); action_it++)
         {
            total_bytes += (*action_it)->GetBytesToSync();
         }

         uint64 bytes_processed = 0;
         for (Actions::const_iterator action_it=actions.begin(); action_it!=actions.end(); action_it++)
         {
            ActionPtr action = *action_it;
            if (!m_settings.info_mode)
            {
               if (!action->Do())
               {
                  TCERRORS("FileSync", "-----------" << action->GetErrorMessage() << "-----------");
                  continue;
               }
            }

            bytes_processed += (*action_it)->GetBytesToSync();

            if (status_displayer)
            {
               //TCINFOS("FileSync", (*action_it)->GetActionString() << " " << (*action_it)->GetSource()->GetName() << " -> " << (*action_it)->GetDestination()->GetName());
               status_displayer->SetProgress(0, bytes_processed, total_bytes);
            }
            else
            {
               uint64 percent = (100 * bytes_processed) / total_bytes;
               if (old_percent != percent)
               {
                  TCINFOS("FileSync", percent << "% done.");
                  old_percent = percent;
               }
            }
         }

         if (status_displayer) status_displayer->SetStatusText("");
         TCINFO("FileSync", "Syncing destination files done.");

         return true;
      }

      const Actions& Syncronizer::GetActions() const
      {
         return m_action_generator.GetActions();
      }
   }
}
