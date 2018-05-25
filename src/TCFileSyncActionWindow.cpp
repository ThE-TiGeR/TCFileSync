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
//  $Id: TCFileSyncActionWindow.cpp,v 5121974dd9eb 2012/09/17 19:34:55 thomas $
//*******************************************************************************

#include "TCFileSyncActionWindow.h"

#include "TCFileName.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace file_sync
   {

      //---------------------------------------------------------------
      //
      //  Derived checkbox table item
      //
      //---------------------------------------------------------------
      class CheckTableItem : public FX::FXTableItem
      {
         FXDECLARE(CheckTableItem)

      protected:
         CheckTableItem() {}

      private:
         FX::FXTable        *Table;
         FX::FXObject        *Target;
         FX::FXSelector        Selector;

         FX::FXCheckButton  *check;

         virtual void drawContent(const FX::FXTable* table,FX::FXDC& dc,FX::FXint
            x,FX::FXint y,FX::FXint w,FX::FXint h) const;

      public:
         long onCheck(FX::FXObject *, FX::FXSelector, void *);

         enum 
         {
            ID_CHECK = 1,
            ID_LAST
         };

      public:
         CheckTableItem(FX::FXTable *table, FX::FXIcon *ic=0, void *ptr=0);
         virtual ~CheckTableItem(void) { delete check; }

         void setCheck(FX::FXbool state=TRUE, FX::FXbool notify=FALSE) {
            check->setCheck(state, notify); }
         FX::FXbool getCheck() const { return check->getCheck(); }
      };


      //---------------------------------------------------------------
      //    CheckTableItem
      //---------------------------------------------------------------
      FXDEFMAP(CheckTableItem) CheckTableItemMap[] = 
      {
         FXMAPFUNC(FX::SEL_COMMAND, CheckTableItem::ID_CHECK, CheckTableItem::onCheck),
      };

      FXIMPLEMENT(CheckTableItem, FX::FXTableItem, CheckTableItemMap, ARRAYNUMBER(CheckTableItemMap))

      // Construct new table item
      CheckTableItem::CheckTableItem(FX::FXTable *table, FX::FXIcon *ic, void *ptr)
      :FX::FXTableItem(FX::FXString::null, ic ,ptr)
      {
         Table = table;
         Target = table->getTarget();
         Selector = table->getSelector();
         check = new FX::FXCheckButton(table, FX::FXString::null, this, ID_CHECK);
         check->setBackColor(table->getBackColor());
         //check->create();
      }

      //
      //    drawContent - override
      //
      void CheckTableItem::drawContent(const FX::FXTable*, FX::FXDC &,
         FX::FXint x, FX::FXint y, FX::FXint w, FX::FXint h) const
      {
         check->position(x+1,y+1,w-2,h-2);
         return;
      }

      //
      //    onCheck
      //
      long CheckTableItem::onCheck(FX::FXObject *, FX::FXSelector, void *)
      {
         if (Target) 
         {
            FX::FXTableRange tablerange;
            tablerange.fm.row = tablerange.to.row = Table->rowAtY(check->getY());
            tablerange.fm.col = tablerange.to.col = Table->colAtX(check->getX());
            //fxmessage("[%d,%d] = %p\n", tablerange.fm.row, tablerange.fm.col, vp);
            Target->handle(this, FXSEL(FX::SEL_REPLACED,Selector), &tablerange);
         }
         return 1;
      }

      FXDEFMAP(ActionWindow) ActionWindowMap[] =
      {
         //________Message_Type_________ID_____________________Message_Handler_______
         FXMAPFUNC(FX::SEL_COMMAND,      ActionWindow::ID_SYNC,          ActionWindow::OnCmdSync),
         FXMAPFUNC(FX::SEL_UPDATE,       ActionWindow::ID_SYNC,          ActionWindow::OnUpdSync),
      };

      TCGUIBASE_IMPLEMENT(ActionWindow, FX::FXTopWindow, ActionWindowMap, ARRAYNUMBER(ActionWindowMap))

      ActionWindow::ActionWindow(FX::FXWindow* window, const Actions& actions)
         :FX::FXTopWindow(window, "Action Window",
         0, 0, FX::DECOR_ALL, 500, 50, 400, 0, 0, 0, 0, 0, 0, 0)
      {
         // Table
         m_table=new FX::FXTable(this ,this, ID_TABLE, FX::TABLE_COL_SIZABLE|FX::TABLE_READONLY|FX::LAYOUT_FILL_X|FX::LAYOUT_FILL_Y,0,0,0,0, 2,2,2,2);
         m_table->setVisibleRows(10);
         m_table->setVisibleColumns(5);
         m_table->setRowHeaderMode(0);
         m_table->setTableSize(FX::FXint(actions.size()),5);

         m_table->setColumnJustify(0, FX::FXHeaderItem::LEFT);
         m_table->setColumnJustify(1, FX::FXHeaderItem::LEFT);
         m_table->setColumnJustify(2, FX::FXHeaderItem::RIGHT);
         m_table->setColumnJustify(3, FX::FXHeaderItem::RIGHT);

         m_table->setColumnText(0, "Active");
         m_table->setColumnText(1, "Action");
         m_table->setColumnText(2, "FileName");
         m_table->setColumnText(3, "Source Directory");
         m_table->setColumnText(4, "Destination Directory");

         // Initialize scrollable part of m_table
         for(uint32_t r=0; r<std::min(actions.size(), Actions::size_type(500)); r++)
         {
            ActionPtr action = actions[r];
            const FileInfo* source      = action->GetSource();
            const FileInfo* destination = action->GetDestination();

            m_table->setItem(r, 0, new CheckTableItem(m_table));
            m_table->setItemText(r, 1, action->GetActionString().c_str());
            m_table->setItemText(r, 2, file_name::GetName(source->GetName()).c_str());
            m_table->setItemText(r, 3, file_name::GetPath(source->GetName()).c_str());
            m_table->setItemText(r, 4, destination ? file_name::GetPath(destination->GetName()).c_str() : "");

            m_table->setItemJustify(r, 1, FX::FXTableItem::LEFT);
            m_table->setItemJustify(r, 2, FX::FXTableItem::LEFT);
            m_table->setItemJustify(r, 3, FX::FXTableItem::RIGHT);
            m_table->setItemJustify(r, 4, FX::FXTableItem::RIGHT);
         }
      }

      ActionWindow::~ActionWindow()
      {

      }

      long ActionWindow::OnCmdSync(FX::FXObject*, FX::FXSelector, void*)
      {
         return 0;
      }


      long ActionWindow::OnUpdSync( FX::FXObject*, FX::FXSelector, void* )
      {
         return 0;
      }

      void ActionWindow::create()
      {
         FX::FXTopWindow::create();
         show(FX::PLACEMENT_SCREEN);
      }
   }
}