#include <updaterTab.hpp>

#include <fmt/core.h>
#include <switch.h>

#include <download.hpp>
#include <constants.hpp>
#include <fs.hpp>
#include <mainActivity.hpp>

using namespace brls::literals;

const std::string updaterTabXml = R"xml(
    <brls:Box
        width="auto"
        height="auto"
        justifyContent="center"
        alignItems="center"
        axis="column">

        <brls:Label
            id="mainLabel"
            width="auto"
            height="auto"
            fontSize="24"
            text="@i18n/text/updater/checking"
            marginBottom="30px"/>

        <brls:Button
            id="continue"
            width="auto"
            height="30"
            style="primary"
            text="Continue"/>

    </brls:Box>
)xml";

UpdaterTab::UpdaterTab()
{
    this->inflateFromXMLString(updaterTabXml);

    BRLS_REGISTER_CLICK_BY_ID("continue", this->onContinueButton);
    ContinButton->hide([]{});

    isNightly = STABLE == std::string("Nightly");

    currentTagVersion = "v{}";
    if (isNightly)
        currentTagVersion = fmt::format(currentTagVersion, VERSION_NUM) + "-nightly";
    else
        currentTagVersion = fmt::format(currentTagVersion, VERSION_NUM);

    newTag = getLatestTag(isNightly);

   if (currentTagVersion != newTag)
   {
        MainLabel->setText("text/updater/new_update"_i18n);
        ContinButton->show([]{});
    }
    else
    {
        MainLabel->setText("text/updater/no_update"_i18n);
        brls::Application::pushActivity(new MainActivity());
        brls::Application::popActivity();
    }
}

brls::View* UpdaterTab::create()
{
    return new UpdaterTab();
}

bool UpdaterTab::onContinueButton(brls::View *view)
{
    std::string urlLink = getLatestDownload(isNightly);
    downloadFile(urlLink, DOWNLOAD_PATH + std::string("/Calculator_NX.nro"));
    fs::copyFile(CONFIG_FORWARDER_PATH, ROMFS_FORWARDER_PATH);
    envSetNextLoad(CONFIG_FORWARDER_PATH, ("\"" + std::string(CONFIG_FORWARDER_PATH) + "\"").c_str());
    return true;
}