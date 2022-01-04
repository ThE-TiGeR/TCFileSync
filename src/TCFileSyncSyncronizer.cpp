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

#include "TCFileSyncSyncronizer.h"

#include "TCOutput.h"
#include "TCString.h"
#include "TCSystem.h"

#include "TCNewEnable.h"

namespace tc::file_sync
{
   Syncronizer::Syncronizer(const Settings& settings, const StatusDisplayerPtr& status_displayer)
      :m_action_generator(settings, status_displayer)
       , m_settings(settings)
       , m_status_displayer(status_displayer)
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
               TCERRORS("FileSync", "------------ " << action->GetActionString() << " failed ---------------" << endl << action->GetErrorMessage());
               continue;
            }
         }

         bytes_processed += action->GetBytesToSync();

         if (m_status_displayer)
         {
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
