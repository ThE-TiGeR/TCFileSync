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

#include "TCApplication.h"
#include "TCFileSyncSyncronizer.h"
#include "TCFileSyncVersion.h"
#include "TCOutput.h"
#include "TCString.h"
#include "TCFileName.h"
#include "TCSystem.h"

#include "TCNewEnable.h"

namespace tc::file_sync
{
   static StreamPtr s_out;

   class MTTraceTarget : public output::PrintTarget
   {
   public:
      MTTraceTarget()
      = default;

      void Print(const char* text) override
      {
         s_out << text << endl << flush;
      }
   };

   class FileSyncApplication final : public Application
   {
   public:
      FileSyncApplication()
      {
         s_out = factory::CreateStdOutStream();

         output::PrintTargetPtr trace_target(new MTTraceTarget);
         output::SetErrorTarget(trace_target);
         output::SetWarningTarget(trace_target);
         output::SetInfoTarget(trace_target);
         output::SetTraceTarget(trace_target);

         s_out << PROGRAM_NAME ": Version " PROGRAM_VERSION_STR << endl
            << "            This product includes: " << TCPRODUCT_STR "-" TCVERSION_STR
            << endl << endl;

         m_settings.info_mode = false;
         m_settings.calc_checksum = false;
         m_settings.backup_folder = "_Older";
         m_settings.num_backups = 5;
      }

      ~FileSyncApplication() override
      {
         output::SetErrorTarget(output::PrintTargetPtr());
         output::SetWarningTarget(output::PrintTargetPtr());
         output::SetInfoTarget(output::PrintTargetPtr());
         output::SetTraceTarget(output::PrintTargetPtr());

         s_out = StreamPtr();
      }

      [[nodiscard]] bool Run() const
      {
         Syncronizer syncronicer(m_settings, StatusDisplayerPtr());
         if (!syncronicer.SetupSyncronisationData())
         {
            TCERRORS("FileSync", "Failed setting up synchronization data");
            return false;
         }
         if (!syncronicer.SyncDestination())
         {
            TCERRORS("FileSync", "Failed syncing directories");
            return false;
         }

         return true;
      }

   protected:
      bool ProcessArguments(const std::vector< std::string >& argv) override
      {
         for (auto it = argv.begin(); it != argv.end(); ++it)
         {
            if (*it == "--source" || *it == "-s")
            {
               ++it;
               m_settings.source = file_name::Simplify(*it);
            }
            else if (*it == "--destination" || *it == "-d")
            {
               ++it;
               m_settings.destination = file_name::Simplify(*it);
            }
            else if (*it == "--backup_folder" || *it == "-b")
            {
               ++it;
               m_settings.backup_folder = file_name::Simplify(*it);
            }
            else if (*it == "--num_backups" || *it == "-n")
            {
               ++it;
               m_settings.num_backups = string::ToUint32(*it);
            }
            else if (*it == "--skipp")
            {
               ++it;
               m_settings.folders_to_skipp.insert(*it);
            }
            else if (*it == "--skipp_ext")
            {
               ++it;
               m_settings.extensions_to_skipp.insert(*it);
            }
            else if (*it == "--ext")
            {
               ++it;
               m_settings.extensions_to_search_for.insert(*it);
            }
            else if (*it == "--info_only" || *it == "-i")
            {
               m_settings.info_mode = true;
            }
            else if (*it == "--calc_checksum" || *it == "-c")
            {
               m_settings.calc_checksum = true;
            }
            else
            {
               DisplayUsage();
               return false;
            }
         }

         if ((m_settings.source.empty() || m_settings.destination.empty()))
         {
            DisplayUsage();
            return false;
         }

         return true;

      }

      std::string GetUsage() override
      {
         return
            "Usage: -s or --source        for source directory\n"
            "       -d or --destination   for destination directory\n"
            "       -b or --backup_folder folder into which to make backups of removed or modified files\n"
            "                             default = _Older\n"
            "       -n or --num_backups   number of backups to keep for one file\n"
            "                             default = 5\n"
            "       --ext                 use only files with specified extension during synchronization, can be set more than once\n"
            "       --skip                folder name to skip during synchronization, can be set more than once\n"
            "       --skip_ext            skip filenames with specified extension during synchronization, can be set more than once\n";
      }

   private:
      Settings m_settings;
   };

}

int main(int narg, char** argv)
{
    {
        std::shared_ptr<tc::file_sync::FileSyncApplication> app(new tc::file_sync::FileSyncApplication);
        if (!app->Init(narg, argv, PROGRAM_NAME, PROGRAM_VERSION_STR, PROGRAM_COMPANY))
        {
            return 1;
        }

        if (!app->Run())
        {
            return 2;
        }
    }

    return 0;
}

