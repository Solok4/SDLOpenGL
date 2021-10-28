#include <vector>
#include <array>
#include <thread>

#include "core/managers/CWindowManager.h"
#include "core/managers/CLayoutManager.h"
#include "core/managers/CModelManager.h"
#include "core/managers/CSceneManager.h"
#include "core/managers/CKeyboardConf.h"
#include "core/managers/CGameplayManager.h"
#include "core/managers/CInitialSetup.h"
#include "core/managers/CFontManager.h"
#include "core/managers/CTextureManager.h"
#include "core/managers/CMaterialManager.h"
#include "core/managers/CEventManager.h"
#include "core/renderer/COpengl.h"
#include "core/shared/Shared.h"

bool Init(int argc, char** argv);
void Destroy();
void Loop();
void PollEvents();
void PreLoop();
