
#include <iostream>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <dlfcn.h>

#include "UIManager.hpp"
#include "cli/CliUIManager.hpp"
#include "util/FSUtils.hpp"
#include "util/Utils.hpp"

using std::string;
using std::cerr;

namespace MCServer {
namespace UI {

UIManager *UIManager::uiManager;

UIManager & UIManager::init(MinecraftServer *server, const string &uiName) {
    if (uiName == "cli") {
        return setupCli(server);
    }

    string fileName;
    bool found(false);
    if (exists((fileName = "ui/" + uiName + ".so"))) {
        found = true;
    }
    else if (exists(fileName = "/etc/mcpp-server/ui/" + uiName + ".so")) {
        found = true;
    }
    else if (exists(fileName = "ui/ui-" + uiName + ".so")) {
        found = true;
    }
    else if (exists(fileName = "/etc/mcpp-server/ui/ui-" + uiName + ".so")) {
        found = true;
    }
    if (!found) {
        invalidOption("Invalid UI type: " + uiName);
    }
    void *handle = dlopen(fileName.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    if (!handle) {
        char *err = dlerror();
        if (err) {
            errorExit("Failed loading UI library in " + fileName + ": " + err);
        }
        else {
            errorExit("Failed loading UI library in " + fileName + ": Unknown error.");
        }
    }

    UIManager * (*newUIManager)(MinecraftServer *) = reinterpret_cast<UIManager * (*)(MinecraftServer *)>(dlsym(handle, "newUIManager"));
    if (!newUIManager) {
        char *err = dlerror();
        if (err) {
            errorExit("Failed loading UI library in " + fileName + ": " + err);
        }
        else {
            errorExit("Failed loading UI library in " + fileName + ": Unknown error.");
        }
    }
    uiManager = newUIManager(server);
    if (!uiManager) {
        cerr << "Failed to instantiate UIManager! Defaulting to CLI.\n";
        return setupCli(server);
    }
    uiManager->setup();
    return *uiManager;
}

UIManager & UIManager::setupCli(MinecraftServer *server) {
    UIManager *cliManager = new CliUIManager(server);
    cliManager->setup();
    return *cliManager;
}

}
}

