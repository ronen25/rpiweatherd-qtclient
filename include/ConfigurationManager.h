#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include <QSettings>

class ConfigurationManager {
public:
    // Access method
    static ConfigurationManager &instance(void);

    // Convinience Methods
    void setValue(const QString &key, const QVariant &value);
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;

    // Getters
    QSettings &settings(void);

    // Deleted operations
    ConfigurationManager(ConfigurationManager const &) = delete;
    void operator=(ConfigurationManager const &) = delete;

private:
    // Cnstr.
    ConfigurationManager();

    // Properties
    QSettings _settings;
};

#endif // CONFIGURATIONMANAGER_H
