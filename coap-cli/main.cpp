/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Copyright (C) 2015-2016 Simon Stuerz <simon.stuerz@guh.guru>           *
 *                                                                         *
 *  This file is part of QtCoap.                                           *
 *                                                                         *
 *  QtCoap is free software: you can redistribute it and/or modify         *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, version 3 of the License.                *
 *                                                                         *
 *  QtCoap is distributed in the hope that it will be useful,              *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with QtCoap. If not, see <http://www.gnu.org/licenses/>.         *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QLoggingCategory>

#include "core.h"
#include "coapnetworkaccessmanager.h"

static QHash<QString, bool> s_loggingFilters;

static void loggingCategoryFilter(QLoggingCategory *category)
{
    if (s_loggingFilters.contains(category->categoryName())) {
        bool debugEnabled = s_loggingFilters.value(category->categoryName());
        category->setEnabled(QtDebugMsg, debugEnabled);
        category->setEnabled(QtWarningMsg, true);
    } else {
        category->setEnabled(QtDebugMsg, true);
        category->setEnabled(QtWarningMsg, true);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication application(argc, argv);

    application.setOrganizationName("guh");
    application.setApplicationName("coap-cli");
    application.setApplicationVersion("1.0.0");

    QCommandLineParser parser;
    parser.addHelpOption();
    QString applicationDescription = QString("\ncoap-cli is a command line tool which allowes to interact with a CoAP server.\n"
                                             "Version: %1\n"
                                             "Copyright %2 2016 Simon Stürz <stuerz.simon@gmail.com>\n"
                                             "Released under the GNU GENERAL PUBLIC LICENSE Version 3.").arg(application.applicationVersion()).arg(QChar(0xA9));
    parser.setApplicationDescription(applicationDescription);
    QCommandLineOption payloadOption(QStringList() << "p" << "payload", "The payload data you want to send.", "payload");
    parser.addOption(payloadOption);
    QCommandLineOption observeOption(QStringList() << "o" << "observe", "Observe the given resource.");
    parser.addOption(observeOption);
    QCommandLineOption discoverOption(QStringList() << "d" << "discover", "Discover the resources of the server.");
    parser.addOption(discoverOption);
    QCommandLineOption verboseOption(QStringList() << "v" << "verbose", "Print the whole communication process.");
    parser.addOption(verboseOption);

    parser.addPositionalArgument("method", "Method you want to use. Possible values are:\n    get (default)\n    put\n    post\n    delete", "[method]");
    parser.addPositionalArgument("url", "URL of the CoAP resource.");
    parser.process(application);

    QString urlString;
    QString methodString = "get";
    QByteArray payload;

    // check positional arguments
    QStringList positionalArguments = parser.positionalArguments();
    if (positionalArguments.isEmpty()) {
        qWarning() << "Argument 'url' missing.\n";
        parser.showHelp();
    }

    // get positional arguments
    if (positionalArguments.count() == 1) {
        urlString = positionalArguments.first();
    } else if (positionalArguments.count() == 2) {
        methodString = positionalArguments.first();
        urlString = positionalArguments.last();
    }

    // Check given URL
    QUrl url = QUrl(urlString);
    if (!url.isValid() || url.isEmpty()) {
        qWarning() << "Invalid url" << urlString;
        exit(-1);
    }
    if (url.scheme() != "coap" && !url.scheme().isEmpty()) {
        qWarning() << "Invalid url scheme" << url.scheme() << endl;
        exit(-1);
    }

    if (url.scheme().isEmpty()) {
        qWarning() << "Warning: Missing url scheme -> using 'coap'";
        url.setScheme("coap");
    }

    if (parser.isSet(verboseOption)) {
        s_loggingFilters.insert("CoAP", true);
    } else {
         s_loggingFilters.insert("CoAP", false);
    }

    // install logging filter
    QLoggingCategory::installFilter(loggingCategoryFilter);

    Core core;

    // pars options
    if (parser.isSet(payloadOption)) {
        payload = parser.value(payloadOption).toUtf8();
    }

    if (parser.isSet(observeOption) && parser.isSet(discoverOption)) {
        qWarning() << "Error: The resource can not be discovered and observed." << endl << "Please use either '-d' or '-o', not both." << endl;
        parser.showHelp();
    }

    if (parser.isSet(observeOption)) {
        core.observe(url);
    } else if (parser.isSet(discoverOption)) {
        core.discover(url);
    } else {
        core.performRequest(url, methodString, payload);
    }

    return application.exec();
}

