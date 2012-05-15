/**************************************************************************
Copyright (C) 2000 - 2010 Novell, Inc.
All Rights Reserved.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

**************************************************************************/

/*---------------------------------------------------------------------\
|								       |
|		       __   __	  ____ _____ ____		       |
|		       \ \ / /_ _/ ___|_   _|___ \		       |
|			\ V / _` \___ \ | |   __) |		       |
|			 | | (_| |___) || |  / __/		       |
|			 |_|\__,_|____/ |_| |_____|		       |
|								       |
|				core system			       |
|							 (C) SuSE GmbH |
\----------------------------------------------------------------------/

  File:		YCommandLine.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YCommandLine_h
#define YCommandLine_h

#include <string>
#include "ImplPtr.h"

using std::string;
class YCommandLinePrivate;


/**
 * Utility class to access /proc/<pid>/cmdline to retrieve argc and argv
 **/
class YCommandLine
{
public:
    /**
     * Constructor. This will read /proc/<pid>/cmdline of this process.
     **/
    YCommandLine();

    /**
     * Destructor.
     **/
    ~YCommandLine();

    /**
     * Return the number of arguments in the command line.
     * Remember that the command itself (the binary of the process) is
     * included, so a value of 1 (not 0!) means "no additional arguments". 
     **/
    int argc() const;

    /**
     * Return the arguments in a C compatible fashion: An array of pointers to
     * characters. The data are copied with strdup(), so they are valid beyond
     * the life time of this object (but OTOH should be released with free() at
     * some point).
     **/
    char ** argv() const;

    /**
     * Alias for argc() for those who like a more C++ -like syntax.
     **/
    int size() const { return argc(); }
    
    /**
     * Return command line argument no. 'index' (from 0 on).
     *
     * This might throw an YUIIndexOutOfRangeException.
     **/
    string arg( int index ) const;

    /**
     * Return command line argument no. 'index' (from 0 on) as operator[]:
     *
     * for ( int i=0; i < cmdLine.argc(); i++ )
     *     cout << cmdLine[i]  << endl;
     * 
     * This might throw an YUIIndexOutOfRangeException.
     **/
    string operator[]( int index ) const
	{ return arg( index ); }

    /**
     * Add a command line argument (at the end of the existing ones).
     **/
    void add( const string & arg );

    /**
     * Remove command line argument no. 'index' (from 0 on).
     *
     * This might throw an YUIIndexOutOfRangeException.
     **/
    void remove( int index );

    /**
     * Replace command line argument no. 'index' (from 0 on) with 'arg'.
     *
     * This might throw an YUIIndexOutOfRangeException.
     **/
    void replace( int index, const string & arg );

    /**
     * Find a command line argument 'argName' ("-display" etc.).
     * Notice that leading minus signs must be specified in 'argName'.
     * Since argv[0] is the program name, the search starts from argv[1].
     *
     * Return the position of 'argName' (from 0 on) or -1 if not found.
     **/
    int find( const string & argName ) const;

    
private:

    ImplPtr<YCommandLinePrivate> priv;
};


#endif // YCommandLine_h