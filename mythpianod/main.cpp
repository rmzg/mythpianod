/*
Copyright (c) 2012
Michael R. Hines <michael@hinespot.com>
Modified from: Doug Turner < dougt@dougt.org >

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

// C++ headers
#include <unistd.h>

// QT headers
#include <QApplication>

// MythTV headers
#include <mythcontext.h>
#include <mythplugin.h>
#include <mythpluginapi.h>
#include <mythversion.h>
#include <mythmainwindow.h>

// MythPianod headers
#include "mythpianod.h"

using namespace std;

static MythPianoService* gMythPianoService = NULL;
MythPianoService * GetMythPianoService()
{
  if (!gMythPianoService)
    gMythPianoService = new MythPianoService();

  return gMythPianoService;
}

void runPandora(void);
int  RunPandora(void);
void setupKeys(void);


void setupKeys(void)
{
  REG_JUMP("MythPianod", QT_TRANSLATE_NOOP("MythPianod",
					    "Sample plugin"), "", runPandora);

  REG_KEY("MythPianod", "VOLUMEDOWN",  "Volume down", "[,{,F10,Volume Down");
  REG_KEY("MythPianod", "VOLUMEUP",    "Volume up",   "],},F11,Volume Up");
  REG_KEY("MythPianod", "PLAY",        "Play",        "p");
  REG_KEY("MythPianod", "PAUSE",       "Pause",        " ");
  REG_KEY("MythPianod", "NEXTTRACK",   "Move to the next track", ",,<,Q,Home");
}

int mythplugin_init(const char *libversion)
{
  //if (!gContext->TestPopupVersion("mythpianod",
					//libversion,
					//MYTH_BINARY_VERSION))
    //return -1;
  setupKeys();
  return 0;
}

void runPandora(void)
{
  RunPandora();
}

int RunPandora()
{
  // Setup Piano Service
  MythPianoService *service = GetMythPianoService();

  /*
  showPopupDialog();
  GetMythMainWindow()->GetMainStack()->PopScreen(false, true);
  */
  // try logging in here.  If it works, then don't show the login dialog.
  if (service->Login() != 0) {
    showLoginDialog();
    return 0;
  }

  if(service->GetCurrentStation() == "") {
	  showStationSelectDialog();
  } else {
	  showPlayerDialog();
  }
  return 0;
}

int mythplugin_run(void)
{
  return RunPandora();
}

int mythplugin_config(void)
{
  return -1;
}

int showLoginDialog()
{
  MythScreenStack *mainStack = GetMythMainWindow()->GetMainStack();
  MythPianodConfig *config = new MythPianodConfig(mainStack, "pandoraconfig");
  if (config->Create()) {
    mainStack->AddScreen(config);
    return 0;
  } else {
    delete config;
    return -1;
  }
  return 0;
}

int showPopupDialog()
{
  MythScreenStack *mainStack = GetMythMainWindow()->GetMainStack();
  MythPianodPopup *popup = new MythPianodPopup(mainStack, "popup");
  if (popup->Create()) {
    mainStack->AddScreen(popup);
    return 0;
  } else {
    delete popup;
    return -1;
  }
  return 0;
}


int showStationSelectDialog()
{
  MythScreenStack *mainStack = GetMythMainWindow()->GetMainStack();
  MythPianodStationSelect *select = new MythPianodStationSelect(mainStack, "pandorastations");
  if (select->Create()) {
    mainStack->AddScreen(select);
    return 0;
  } else {
    delete select;
    return -1;
  }
  return 0;
}

int showPlayerDialog()
{
  MythScreenStack *mainStack = GetMythMainWindow()->GetMainStack();
  MythPianod *mythpianod = new MythPianod(mainStack, "pandora");
  
  if (mythpianod->Create()){
    mainStack->AddScreen(mythpianod);
  } else {
    delete mythpianod;
    return -1;
  }
  return 0;
}
