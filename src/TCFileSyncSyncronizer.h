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
#pragma once

#include "TCFileSyncActionGenerator.h"
#include "TCFileSyncStatusDisplayer.h"

namespace tc::file_sync
{
   class Syncronizer final
   {
   public:
      Syncronizer(const Settings& settings, const StatusDisplayerPtr& status_displayer);

      bool SetupSyncronisationData();
      [[nodiscard]] const Actions& GetActions() const;
      bool SyncDestination();

   private:
      ActionGenerator m_action_generator;
      Settings m_settings;
      StatusDisplayerPtr m_status_displayer;
   };
}
