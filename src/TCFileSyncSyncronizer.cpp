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
//  $Id: TCFileSyncSyncronizer.cpp 961 2010-03-27 23:26:56Z the_____tiger $
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

namespace TC
{
   namespace FileSync
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

      bool Syncronizer::SyncDestination()
      {
         Actions actions = m_action_generator.GetActions();
         Actions::const_iterator action_it;

         // find total bytes to process
         uint64 total_bytes = 0;
         for (action_it=actions.begin(); action_it!=actions.end(); action_it++)
         {
            total_bytes += (*action_it)->GetBytesToSync();
         }

         uint64 bytes_processed = 0;
         for (action_it=actions.begin(); action_it!=actions.end(); action_it++)
         {
            ActionPtr action = *action_it;
            //TCINFOS("FileSync", action->GetInfo() << " ... ");
            if (!m_settings.info_mode)
            {
               if (!action->Do())
               {
                  //TCERRORS("FileSync", action->GetInfo() << " failed !!!!!!!!!");
                  TCERRORS("FileSync", "-----------" << action->GetErrorMessage() << "-----------");
                  continue;
               }
            }

            bytes_processed += (*action_it)->GetBytesToSync();
            uint64 percent = (100 * bytes_processed) / total_bytes;
            TCINFOS("FileSync", percent << "% done.");
            //TCINFOS("FileSync", action->GetInfo() << " done.");
         }
         return true;
      }

      const Actions& Syncronizer::GetActions() const
      {
         return m_action_generator.GetActions();
      }
   }
}
