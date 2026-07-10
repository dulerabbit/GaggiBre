#pragma once
#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H
#include "PluginManager.h"
#include <FS.h>
#include <display/core/Settings.h>
#include <display/core/utils.h>
#include <display/models/profile.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

class ProfileManager {
  public:
    ProfileManager(fs::FS *fs, String dir, Settings &settings, PluginManager *plugin_manager);
    ~ProfileManager();

    void setup();
    std::vector<String> listProfiles();
    bool loadProfile(const String &uuid, Profile &outProfile);
    bool saveProfile(Profile &profile, bool reselect = true);
    bool deleteProfile(const String &uuid);
    bool profileExists(const String &uuid);
    void selectProfile(const String &uuid);
    Profile &getSelectedProfile();
    bool loadSelectedProfile(Profile &outProfile);
    std::vector<String> getFavoritedProfiles(bool validate = false);

    void addFavoritedProfile(String id);
    void removeFavoritedProfile(String id);

  private:
    class Lock {
      public:
        explicit Lock(SemaphoreHandle_t handle) : _handle(handle) {
            if (_handle != nullptr) {
                xSemaphoreTakeRecursive(_handle, portMAX_DELAY);
            }
        }
        ~Lock() {
            if (_handle != nullptr) {
                xSemaphoreGiveRecursive(_handle);
            }
        }
        Lock(const Lock &) = delete;
        Lock &operator=(const Lock &) = delete;

      private:
        SemaphoreHandle_t _handle;
    };

    Profile selectedProfile{};
    PluginManager *_plugin_manager;
    Settings &_settings;
    fs::FS *_fs;
    String _dir;
    SemaphoreHandle_t _lock = nullptr;

    bool ensureDirectory() const;
    String profilePath(const String &uuid) const;
    void migrate();
};

#endif // PROFILEMANAGER_H
