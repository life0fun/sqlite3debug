# How to step through sqlite3 in a debugger

we try to create a local dev environment on my OSX Sierra so I can debug sqlite3.
Tried with Eclipse Neo on my OSX Sierra with the following setup, no luck.

https://www.ics.uci.edu/~pattis/common/handouts/macmingweclipse/allexperimental/mac-gdb-install.html

Finally, made it work with xcode.
folder sqlitetest containers the xcode project for this. download latest xcode and import the project, you can setup breakpoints and step through.

## database
we use chinook.db from sqlite tutorial.

the current working dir in mac is somewhere under $HOME/Library/Developer/Xcode/DerivedData/sqlitetest-fuhccdahfchkkphkkellpeacfgxk/Build/Products/Debug.
It is not at the project home. wierd.


## Sqlite Btree Pages

there 4 types of btree pages, namely,

    table btree interior page with cell [left-child-page-no, varint-key]
    table btree leaf page with cell [varint-payload-size, rowId, payload-header(col schema), row-content]
    index btree interior page with cell
    index btree leaf page with cell

sqlite is using b+ tree where all row data is stored in leaf page cell, and table page interior cells only store [left-child-page, key].

Cells in page are sorted by their keys. 2 bytes cell pointer stores offset of cell content area from the end of page. 

When insert new row, append row data, and re-arrange cell idx array with
 
    static int editPage(
      MemPage *pPg,                   /* Edit this page */
      int iOld,                       /* Index of first cell currently on page */
      int iNew,                       /* Index of new first cell on page */
      int nNew,                       /* Final number of cells on page */
      CellArray *pCArray              /* Array of cells and sizes */
    ){
       memmove(&pCellptr[nAdd*2], pCellptr, nCell*2);
    }

The other path is balance btree node using balance_quick, balance_nonroot, and balance_deeper.

Simple example of balance_quick.

    static int balance_quick(MemPage *pParent, MemPage *pPage, u8 *pSpace){
      rc = allocateBtreePage(pBt, &pNew, &pgnoNew, 0, 0);
      // find divider cell [left-child-page-no, key] to insert into parent interior page, 
      pCell = findCell(pPage, pPage->nCell-1);
      // copy key into pOut.
      while( ((*(pOut++) = *(pCell++))&0x80) && pCell<pStop );
      // insert divide cell into parent
      insertCell(pParent, pParent->nCell, pSpace, (int)(pOut-pSpace),
                   0, pPage->pgno, &rc);
    
      /* Set the right-child pointer of pParent to point to the new page. */
      put4byte(&pParent->aData[pParent->hdrOffset+8], pgnoNew);
  
      /* Release the reference to the new page. */
      releasePage(pNew);
    }



https://www.sqlite.org/fileformat2.html
https://digitalforensicforest.wordpress.com/2015/07/27/sqlite-data-carving-a-way-to-trace/
http://forensicsfromthesausagefactory.blogspot.com/2011/05/analysis-of-record-structure-within.html
