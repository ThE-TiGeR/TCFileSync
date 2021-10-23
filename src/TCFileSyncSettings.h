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

#include "TCTypes.h"

#include <set>
#include <string>

namespace tc::file_sync
{
   class Settings final
   {
   public:
      bool info_mode{};
      bool calc_checksum{};
      std::string source;
      std::string destination;

      uint32_t num_backups{};
      std::string backup_folder;

      std::set< std::string > folders_to_skipp;
      std::set< std::string > extensions_to_search_for;
      std::set< std::string > extensions_to_skipp;
   };
}


