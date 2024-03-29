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
      bool create_directories{};
      std::wstring source;
      std::wstring destination;

      uint32_t num_backups{};
      std::wstring backup_folder;

      std::set< std::wstring > files_and_folders_to_skipp;
      std::set< std::wstring > destination_files_and_folders_to_ignore;
      std::set< std::wstring > extensions_to_search_for;
      std::set< std::wstring > extensions_to_skipp;
   };
}


