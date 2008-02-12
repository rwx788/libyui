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

  File:		YQOptionalWidgetFactory.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#include "YQOptionalWidgetFactory.h"
#include "YQApplication.h"
#include "YUIException.h"
#include "YQPackageSelectorPlugin.h"
#include "YQWizard.h"
#include "YQTimezoneSelector.h"

#define THROW_UNSUPPORTED( WIDGET_TYPE ) \
    YUI_THROW( YUIUnsupportedWidgetException( WIDGET_TYPE ) );	\
    return 0



YQOptionalWidgetFactory::YQOptionalWidgetFactory()
    : YOptionalWidgetFactory()
{
    // NOP
}

YQOptionalWidgetFactory::~YQOptionalWidgetFactory()
{
    // NOP
}



bool YQOptionalWidgetFactory::hasWizard()
{
    return true;
}

YWizard *
YQOptionalWidgetFactory::createWizard( YWidget *   	parent,
				       const string & 	backButtonLabel,
				       const string & 	abortButtonLabel,
				       const string & 	nextButtonLabel,
				       YWizardMode 	wizardMode )
{
    YQWizard * wizard = new YQWizard( parent,
				      backButtonLabel,
				      abortButtonLabel,
				      nextButtonLabel,
				      wizardMode );
    YUI_CHECK_NEW( wizard );

    return wizard;
}



bool YQOptionalWidgetFactory::hasDumbTab()
{
    return true;
}

YQDumbTab *
YQOptionalWidgetFactory::createDumbTab( YWidget * parent )
{
    YQDumbTab * dumbTab = new YQDumbTab( parent );
    YUI_CHECK_NEW( dumbTab );

    return dumbTab;
}



bool YQOptionalWidgetFactory::hasSlider()
{
    return true;
}

YQSlider *
YQOptionalWidgetFactory::createSlider( YWidget *	parent,
				      const string &	label,
				      int 		minVal,
				      int 		maxVal,
				      int 		initialVal )
{
    YQSlider * slider = new YQSlider( parent, label, minVal, maxVal, initialVal );
    YUI_CHECK_NEW( slider );

    return slider;
}



bool YQOptionalWidgetFactory::hasDateField()
{
    return true;
}

YQDateField *
YQOptionalWidgetFactory::createDateField( YWidget * parent, const string & label )
{
    YQDateField * dateField = new YQDateField( parent, label );
    YUI_CHECK_NEW( dateField );

    return dateField;
}



bool YQOptionalWidgetFactory::hasTimeField()
{
    return true;
}

YQTimeField *
YQOptionalWidgetFactory::createTimeField( YWidget * parent, const string & label )
{
    YQTimeField * timeField = new YQTimeField( parent, label );
    YUI_CHECK_NEW( timeField );

    return timeField;
}



bool YQOptionalWidgetFactory::hasBarGraph()
{
    return true;
}

YQBarGraph *
YQOptionalWidgetFactory::createBarGraph( YWidget * parent )
{
    YQBarGraph * barGraph = new YQBarGraph( parent );
    YUI_CHECK_NEW( barGraph );

    return barGraph;
}



bool YQOptionalWidgetFactory::hasPatternSelector()
{
    return true;
}

YQPatternSelector *
YQOptionalWidgetFactory::createPatternSelector( YWidget * parent, long modeFlags )
{
    YQPackageSelectorPlugin * plugin = YQApplication::packageSelectorPlugin();

    if ( plugin )
	return plugin->createPatternSelector( parent, modeFlags );
    else
	return 0;
}



bool YQOptionalWidgetFactory::hasSimplePatchSelector()
{
    return true;
}

YQSimplePatchSelector *
YQOptionalWidgetFactory::createSimplePatchSelector( YWidget * parent, long modeFlags )
{
    YQPackageSelectorPlugin * plugin = YQApplication::packageSelectorPlugin();

    if ( plugin )
	return plugin->createSimplePatchSelector( parent, modeFlags );
    else
	return 0;
}



bool YQOptionalWidgetFactory::hasMultiProgressMeter()
{
    return true;
}

YQMultiProgressMeter *
YQOptionalWidgetFactory::createMultiProgressMeter( YWidget * parent, YUIDimension dim, const vector<float> & maxValues )
{
    YQMultiProgressMeter * multiProgressMeter = new YQMultiProgressMeter( parent, dim, maxValues );
    YUI_CHECK_NEW( multiProgressMeter );

    return multiProgressMeter;
}



bool YQOptionalWidgetFactory::hasPartitionSplitter()
{
    return true;
}

YQPartitionSplitter *
YQOptionalWidgetFactory::createPartitionSplitter( YWidget * 		parent,
						  int 			usedSize,
						  int 			totalFreeSize,
						  int 			newPartSize,
						  int 			minNewPartSize,
						  int 			minFreeSize,
						  const string &	usedLabel,
						  const string &	freeLabel,
						  const string &	newPartLabel,
						  const string &	freeFieldLabel,
						  const string &	newPartFieldLabel )
{
    YQPartitionSplitter * partitionSplitter = new YQPartitionSplitter(	parent,
									usedSize,
									totalFreeSize,
									newPartSize,
									minNewPartSize,
									minFreeSize,
									usedLabel,
									freeLabel,
									newPartLabel,
									freeFieldLabel,
									newPartFieldLabel );
    YUI_CHECK_NEW( partitionSplitter );

    return partitionSplitter;
}



bool YQOptionalWidgetFactory::hasDownloadProgress()
{
    return true;
}

YQDownloadProgress *
YQOptionalWidgetFactory::createDownloadProgress( YWidget *	parent,
						 const string &	label,
						 const string & filename,
						 YFileSize_t	expectedSize )
{
    YQDownloadProgress * downloadProgress = new YQDownloadProgress( parent,
								    label,
								    filename,
								    expectedSize );
    YUI_CHECK_NEW( downloadProgress );

    return downloadProgress;
}

bool YQOptionalWidgetFactory::hasTimezoneSelector()
{
    return true;
}

YTimezoneSelector *
YQOptionalWidgetFactory::createTimezoneSelector( YWidget * parent,
                                                 const string & pixmap,
                                                 const map<string,string> & timezones )
{
    return new YQTimezoneSelector( parent, pixmap, timezones );
}

