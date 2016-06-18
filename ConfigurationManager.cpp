#include "ConfigurationManager.h"

ConfigurationManager &ConfigurationManager::instance() {
    static ConfigurationManager inst;
    return inst;
}

void ConfigurationManager::setValue(const QString &key, const QVariant &value) {
    _settings.setValue(key, value);
}

QVariant ConfigurationManager::value(const QString &key, const QVariant &defaultValue) const {
    return _settings.value(key, defaultValue);
}

QSettings &ConfigurationManager::settings() {
    return _settings;
}

ConfigurationManager::ConfigurationManager() :
    _settings("RonenL", "rpiweatherd-qtclient") { }
