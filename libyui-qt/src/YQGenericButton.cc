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

  File:	      YQGenericButton.cc

  Author:     Stefan Hundhammer <sh@suse.de>

/-*/


#include <qpushbutton.h>
#include <qsize.h>
#include <qevent.h>
//Added by qt3to4:
#include <qpixmap.h>
#include <qevent.h>
#define y2log_component "qt-ui"
#include <ycp/y2log.h>

#include "utf8.h"
#include "YQUI.h"
#include "YEvent.h"
#include "YQGenericButton.h"
#include "YQDialog.h"


YQGenericButton::YQGenericButton( YWidget *		parent,
				  const string &	label )
    : QWidget( (QWidget *) parent->widgetRep() )
    , YPushButton( parent, label )
    , _dialog( 0 )
    , _qPushButton( 0 )
{
    setWidgetRep( 0 );
}


void YQGenericButton::setQPushButton( QPushButton * pb )
{
    _qPushButton = pb;
    _qPushButton->setAutoDefault( true );
    _qPushButton->installEventFilter( this );
    _qPushButton->setAutoDefault( true );

    YPushButton::setLabel( toUTF8 ( _qPushButton->text() ) );
}


YQGenericButton::~YQGenericButton()
{
    if ( _dialog ) // If we don't have one any more, don't bother
    {
	if ( _dialog->focusButton() == this )
	    _dialog->losingFocus( this );

	if ( _dialog->defaultButton() == this )
	    _dialog->setDefaultButton(0);
    }
}

void
YQGenericButton::forgetDialog()
{
   _dialog = 0;
}

YQDialog *
YQGenericButton::dialog()
{
    if ( ! _dialog )
    {
	YDialog * yDialog = findDialog();

	if ( yDialog )
	    _dialog = dynamic_cast<YQDialog *> (yDialog);

	YUI_CHECK_PTR( _dialog );
    }

    return _dialog;
}


void YQGenericButton::setEnabled( bool enabled )
{
    if ( _qPushButton )
	_qPushButton->setEnabled( enabled );

    YWidget::setEnabled( enabled );
}


bool YQGenericButton::isEnabled() const
{
    return _qPushButton ? _qPushButton->isEnabled() : false;
}


void YQGenericButton::setIcon( const string & iconName )
{
    if ( ! _qPushButton )
    {
	y2error( "NULL button (icon %s)", iconName.c_str() );
	return;
    }

    QString qIconName = fromUTF8( iconName );

    if ( qIconName.isEmpty() )
    {
	_qPushButton->setIcon( QIcon() );
	return;
    }

    qIconName = QString( ICONDIR ) + "/" + qIconName;
    QPixmap icon( qIconName );

    if ( icon.isNull() )
	y2warning( "Can't load icon '%s'", qPrintable(qIconName) );
    else
	_qPushButton->setIcon( icon );
}


void YQGenericButton::setLabel( const QString & label )
{
    if ( _qPushButton )
	_qPushButton->setText( label );
    else
	y2error( "NULL button '%s'", qPrintable(label) );

    YPushButton::setLabel( toUTF8( label ) );
}


void YQGenericButton::setLabel( const string & label )
{
    if ( _qPushButton )
	_qPushButton->setText( fromUTF8( label ) );
    else
	y2error( "NULL button '%s'", label.c_str() );

    YPushButton::setLabel( label );
}


void YQGenericButton::showAsDefault( bool show )
{
    if ( _qPushButton )
    {
	_qPushButton->setDefault( show );
	_qPushButton->update();
    }
}


bool YQGenericButton::isShownAsDefault() const
{
    return _qPushButton ? _qPushButton->isDefault() : false;
}


QString
YQGenericButton::text() const
{
    return _qPushButton ? _qPushButton->text() : "";
}


void YQGenericButton::activate()
{
    if ( _qPushButton )
	_qPushButton->animateClick();
}


bool YQGenericButton::eventFilter( QObject * obj, QEvent * event )
{
    if ( event )
    {
	if ( event->type() == QEvent::FocusIn )
	{
	    dialog()->gettingFocus( this );
	    return false;	// event processed?
	}
	else if ( event->type() == QEvent::FocusOut )
	{
	    dialog()->losingFocus( this );
	    return false;	// event processed?
	}
	else if ( event->type() == QEvent::MouseButtonRelease )
	{
	    QMouseEvent * mouseEvent = dynamic_cast<QMouseEvent *> (event);

	    if ( mouseEvent && mouseEvent->button() == Qt::RightButton )
	    {
		y2milestone( "Right click on button detected" );
		YQUI::ui()->maybeLeftHandedUser();
	    }
	}
    }


    return QObject::eventFilter( obj, event );
}


bool YQGenericButton::setKeyboardFocus()
{
    if ( ! _qPushButton )
	return false;

    dialog()->gettingFocus( this );
    _qPushButton->setFocus();

    return true;
}


#include "YQGenericButton.moc"
