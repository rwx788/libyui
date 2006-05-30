/*-----------------------------------------------------------*- c++ -*-\
|                                                                      |
|                      __   __    ____ _____ ____                      |
|                      \ \ / /_ _/ ___|_   _|___ \                     |
|                       \ V / _` \___ \ | |   __) |                    |
|                        | | (_| |___) || |  / __/                     |
|                        |_|\__,_|____/ |_| |_____|                    |
|                                                                      |
|                               core system                            |
|                                                        (C) SuSE GmbH |
\----------------------------------------------------------------------/

   File:       NCPkgTable.h

   Author:     Gabriele Strattner <gs@suse.de>
   Maintainer: Michael Andres <ma@suse.de>

/-*/
#ifndef NCPkgTable_h
#define NCPkgTable_h

#include <iosfwd>

#include "NCPadWidget.h"
#include "NCTablePad.h"
#include "NCTable.h"
#include "PkgNames.h"

#include <map>          
#include <string>
#include <utility>      // for STL pair

#include "YQZypp.h"
#include <zypp/ui/Selectable.h>

#include "ObjectStatStrategy.h"


class PackageSelector;


/**
 * This class is used for the first column of the package table
 * which contains the status information of the package (installed,
 * not installed, to be deleted and so on).
 *
 **/
class NCPkgTableTag : public NCTableCol {

  private:

    ZyppStatus status;
    ZyppObj dataPointer;
    // cannot get at it from dataPointer
    ZyppSel selPointer;

    // returns the corresponding string value to given package status
    string statusToStr( ZyppStatus stat ) const;
    
  public:

    NCPkgTableTag( ZyppObj pkgPtr,
		   ZyppSel selPtr,
		   ZyppStatus stat = S_NoInst );

    virtual ~NCPkgTableTag() {}

    virtual void DrawAt( NCursesWindow & w, const wrect at,
			 NCTableStyle & tableStyle,
			 NCTableLine::STATE linestate,
			 unsigned colidx ) const; 

    void setStatus( ZyppStatus  stat ) 	{ status = stat; }
    ZyppStatus getStatus() const   { return status; }
    ZyppObj getDataPointer() const		{ return dataPointer; }
    ZyppSel getSelPointer() const		{ return selPointer; }
};

/**
 * The package table class. Provides methods to fill the table,
 * set the status info and so on.
 * Has a connection to the PackageSelector which is used to do
 * changes which affect other widgets.
 *
 **/
class NCPkgTable : public NCTable {

public:
    enum NCPkgTableType {
	T_Packages,
	T_Availables,
	T_Patches,
	T_Update,
	T_PatchPkgs,
	T_Selections,
	T_Unknown
    };

    enum NCPkgTableListAction {
	A_Install,
	A_DontInstall,
	A_Delete,
	A_DontDelete,
	A_Update,
	A_DontUpdate,
	A_Unknown
    };

    enum NCPkgTableListType {
	L_Changes,
	L_Installed,
	L_Unknown
    };
    
private:

    NCPkgTable & operator=( const NCPkgTable & );
    NCPkgTable            ( const NCPkgTable & );
    
    PackageSelector * packager;		// connection to the PackageSelector,

    ObjectStatStrategy * statusStrategy; 	// particular methods to get the status

    NCPkgTableType tableType;	// the type (e.g. table of packages, patches)
    bool haveInstalledVersion;	// for T_Packages and T_Update
    
    // returns the first column of line with 'index' (the tag)
    NCPkgTableTag * getTag ( const int & index );

protected:


public:

   /**
    * Constructor
    */
    NCPkgTable( NCWidget * parent, const YWidgetOpt & opt );

    virtual ~NCPkgTable();


   /**
    * This method is called to add a line to the package list.
    * @param status The package status (first column of the table)
    * @param elements A vector<string> containing the package data
    * @param objPtr The pointer to the packagemanager object
    * @param objPtr The pointer to the selectable object
    * @return void
    */
    virtual void addLine( ZyppStatus status,
			  const vector<string> & elements,
			  ZyppObj objPtr,
			  ZyppSel slbPtr );

   /**
     * Draws the package list (has to be called after the loop with addLine() calls)
     */ 
   void drawList( ) { return DrawPad(); }
    
   /**
    * Clears the package list
    */
    virtual void itemsCleared();

   /**
    * Changes the contents of a certain cell in table
    * @param index The table line
    * @param column The column
    * @param newtext The new text
    * @eturn void
    */
    virtual void cellChanged( int index, int colnum, const YCPString & newtext );

   /**
    * Returns the contents of a certain cell in table
    * @param index The table line
    * @param column The column
    * @eturn NClabel
    */
    NClabel getCellContents( int index, int colnum );

    /**
     * Handles the events concerning the package table (e.g. scroll the list,
     * change the package status, ...)
     * @param key The key which is pressed
     * @return NCursesEvent
     */
    virtual NCursesEvent wHandleInput( wint_t key );

   /**
     * Sets the member variable PackageSelector *packager
     * @param pkg The PackageSelector pointer
     * @return void
     */
    void setPackager( PackageSelector * pkg ) { packager = pkg; }

    /**
     * Informs the package manager about the status change of
     * the currently selected package and updates the states
     * of all packages in the list
     * @param newstat The new status
     * @param slbPtr The pointer to the object to change 
     * @param objPtr is candidatePtr or what the user selected instead of it.
     * @return bool
     */
    bool changeStatus( ZyppStatus newstat,
		       const ZyppSel & slbPtr,
		       ZyppObj objPtr,
		       bool singleChange );
    
    bool changeObjStatus( int key );

    bool changeListObjStatus( NCPkgTableListAction key );

    bool toggleObjStatus( );

   /**
     * Set the status information if status has changed 
     * @return bool
     */  
    bool updateTable();

    /**
     * Gets the currently displayed package status.
     * @param index The index in package table (the line)
     * @return ZyppStatus
     */ 
    ZyppStatus getStatus( int index );

#ifdef FIXME
    /**
     * Toggles the installation of the source package.
     * @param install 
     * @return bool
     */ 
    bool SourceInstall( bool install );
#endif
    
    /**
     * Sets the type of the table and the status strategy (which means call particular methods
     * to set/get the status for different zypp::ResObjects (zypp::Patch, zypp::Package or available zypp::Package)
     * @param type	The type (see enum NCPkgTableType)
     * @param strategy  The certain strategy (available strategies see ObjectStatStrategy.h).
     * 			Has to be allocated with new - is deleted by NCPkgTable.
     * @return bool
     */
    bool setTableType( NCPkgTableType type, ObjectStatStrategy * strategy ) {
	if ( !strategy )
	    return false;
	
	delete statusStrategy;
	statusStrategy = strategy;
	tableType = type;

	return true;
    }

    /**
     * Gets the data pointer of a certain package.
     * @param index The index in package table (the line)
     * @return ZyppObj
     */ 
    ZyppObj getDataPointer( int index );

    /**
     * Gets the selectable pointer of a certain package.
     * @param index The index in package table (the line)
     * @return ZyppSel
     */ 
    ZyppSel getSelPointer( int index );

   /**
     * Returns the number of lines in the table (the table size)
     * @return unsigned int
     */ 
    unsigned int getNumLines( ) { return myPad()->Lines(); }

    /**
     * Shows default list (packages of default RPM group or patch list)
     * @return bool
     */  
    bool fillDefaultList( );

    /**
     * Fills the header of the table
     * @return void
     */  
    void fillHeader( );

    /**
     * Creates a line in the package table.
     * @param pkgPtr The package pointer
     * @param slbPtr The selectable pointer
     * @return bool
     */  
   bool createListEntry ( ZyppPkg pkgPtr, ZyppSel slbPtr );

   /**
     * Creates a line in the YOU patch table.
     * @param pkgPtr The YOU patch pointer 
     * @return bool
     */  
   bool createPatchEntry ( ZyppPatch pkgPtr,  ZyppSel slbPtr );

   /**
    * Creates a line in the table shwing an info text.
    * @param text The information
    * @return bool
    */   
   bool createInfoEntry ( string text );

   /**
    * Show the corresponding information (e.g. the package description).
    * @return bool
    */     
   bool showInformation ( );


};

///////////////////////////////////////////////////////////////////

#endif // NCPkgTable_h
