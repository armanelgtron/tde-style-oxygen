//////////////////////////////////////////////////////////////////////////////
// exampleconfig.cc
// -------------------
// Config module for Example window decoration
// -------------------
// Copyright (c) 2003 David Johnson <david@usermode.org>
// Please see the header file for copyright and license information.
//////////////////////////////////////////////////////////////////////////////

#include <tdeconfig.h>
#include <tdelocale.h>
#include <tdeglobal.h>
#include <tqbuttongroup.h>
#include <tqgroupbox.h>
#include <tqradiobutton.h>
#include <tqwhatsthis.h>

#include "exampleconfig.h"
#include "configdialog.h"

//////////////////////////////////////////////////////////////////////////////
// ExampleConfig()
// -------------
// Constructor

ExampleConfig::ExampleConfig(TDEConfig* config, TQWidget* parent)
    : TQObject(parent), config_(0), dialog_(0)
{
    // create the configuration object
    config_ = new TDEConfig("twinoxygenrc");
    TDEGlobal::locale()->insertCatalogue("kwin_example_config");

    // create and show the configuration dialog
    dialog_ = new ConfigDialog(parent);
    dialog_->show();

    // load the configuration
    load(config_);

    // setup the connections
    connect(dialog_->titlealign, TQ_SIGNAL(clicked(int)),
            this, TQ_SLOT(selectionChanged(int)));

}

//////////////////////////////////////////////////////////////////////////////
// ~ExampleConfig()
// --------------
// Destructor

ExampleConfig::~ExampleConfig()
{
    if (dialog_) delete dialog_;
    if (config_) delete config_;
}

//////////////////////////////////////////////////////////////////////////////
// selectionChanged()
// ------------------
// Selection has changed

void ExampleConfig::selectionChanged(int)
{
    emit changed();
}

//////////////////////////////////////////////////////////////////////////////
// load()
// ------
// Load configuration data

void ExampleConfig::load(TDEConfig*)
{
    config_->setGroup("General");
    
    TQString value = config_->readEntry("TitleAlignment", "AlignHCenter");
    TQRadioButton *button = (TQRadioButton*)dialog_->titlealign->child(value.latin1());
    if (button) button->setChecked(true);
}

//////////////////////////////////////////////////////////////////////////////
// save()
// ------
// Save configuration data

void ExampleConfig::save(TDEConfig*)
{
    config_->setGroup("General");

    TQRadioButton *button = (TQRadioButton*)dialog_->titlealign->selected();
    if (button) config_->writeEntry("TitleAlignment", TQString(button->name()));
    config_->sync();
}

//////////////////////////////////////////////////////////////////////////////
// defaults()
// ----------
// Set configuration defaults

void ExampleConfig::defaults()
{
    TQRadioButton *button =
        (TQRadioButton*)dialog_->titlealign->child("AlignHCenter");
    if (button) button->setChecked(true);
}

//////////////////////////////////////////////////////////////////////////////
// Plugin Stuff                                                             //
//////////////////////////////////////////////////////////////////////////////

extern "C"
{
    TQObject* allocate_config(TDEConfig * config, TQWidget* parent) {
        return (new ExampleConfig(config, parent));
    }
}

#include "exampleconfig.moc"
