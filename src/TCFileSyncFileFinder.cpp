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

#include "TCFileSyncFileFinder.h"

#include <utility>

#include "TCWFile.h"
#include "TCWFileName.h"
#include "TCOutput.h"
#include "TCSystem.h"

namespace tc
{
    file_sync::FileFinder::FileFinder(Settings settings)
        :m_settings(std::move(settings))
    {
    }

    bool file_sync::FileFinder::Find()
    {
        std::wstring dir_save = wfile::GetDirectory();

        if (!FindSourceFiles()) return false;
        if (!FindDestinationFiles()) return false;

        return wfile::ChangeDirectory(dir_save);
    }

    bool file_sync::FileFinder::FindSourceFiles()
    {
        TCINFOS("FileSync::FileFinder", "Searching source files in " << m_settings.source << " ...");

        m_files_source.clear();
        if (!wfile::ChangeDirectory(m_settings.source)) return false;
        if (!FindRecursiveFiles(L".", m_files_source)) return false;

        for (auto & file_info : m_files_source)
        {
            file_info.second.SetDir(m_settings.source);
        }

        TCINFOS("FileSync::FileFinder", "Found " << m_files_source.size() << " source files in " << m_settings.source);
        return true;
    }

    bool file_sync::FileFinder::FindDestinationFiles()
    {
        TCINFOS("FileSync::FileFinder", "Searching destination files in " << m_settings.destination << " ...");

        // reset setting for destination search because we have to find all
        file_sync::Settings settings = m_settings;
        m_settings.extensions_to_search_for.clear();
        m_settings.extensions_to_skipp.clear();
        m_settings.files_and_folders_to_skipp.clear();

        m_files_destination.clear();
        if (!wfile::ChangeDirectory(m_settings.destination)) return false;
        if (!FindRecursiveFiles(L".", m_files_destination)) return false;

        for (auto & file_info : m_files_destination)
        {
            file_info.second.SetDir(m_settings.destination);
        }

        TCINFOS("FileSync::FileFinder", "Found " << m_files_destination.size() << " destination files in " << m_settings.destination);

        m_settings = settings;

        return true;
    }

    bool file_sync::FileFinder::FindRecursiveFiles(const std::wstring& search_dir, FileInfos& files)
    {
        std::vector<wfile::FileInfo> file_infos_of_dir;
        wfile::GetFileInfosOfDirectory(file_infos_of_dir, search_dir);

        for (auto & file : file_infos_of_dir)
        {
            FileInfo file_info(file);
            file_info.SetName(wfile_name::AddFileNameAndPath(file.name, search_dir));
            if (m_settings.files_and_folders_to_skipp.find(file.name) != m_settings.files_and_folders_to_skipp.end())
            {
               continue;
            }

            if (file_info.IsDirectory())
            {
                // check if we have to ignore this folder
                if (file.name != m_settings.backup_folder)
                {
                    FindRecursiveFiles(file_info.GetName(), files);
                }
                else
                {
                    continue;
                }
            }
            else
            {
                // check if only some predefined extensions should be searched
                if (!m_settings.extensions_to_search_for.empty())
                {
                    std::wstring ext = wfile_name::GetExtension(file_info.GetName());
                    if (m_settings.extensions_to_search_for.find(ext) ==
                        m_settings.extensions_to_search_for.end())
                    {
                        continue;
                    }
                }

                // check if we have to skipp some extensions
                if (!m_settings.extensions_to_skipp.empty())
                {
                    std::wstring ext = wfile_name::GetExtension(file_info.GetName());
                    if (m_settings.extensions_to_skipp.find(ext) !=
                        m_settings.extensions_to_skipp.end())
                    {
                        continue;
                    }
                }
            }

            TCTRACES("FileSync::FileFinder", 10, "Found file " << file_info.GetName() << ".");
            files[file_info.GetName()] = file_info;
        }

        return true;
    }
}
