#ifndef ADMOBMANAGER_H_INCLUDED
#define ADMOBMANAGER_H_INCLUDED

#if defined(__ANDROID__)
#if defined(ENABLE_ADMOB)
#include "firebase/admob.h"
#include "firebase/admob/types.h"
#include "firebase/app.h"
#include "firebase/future.h"
#include "firebase/admob/banner_view.h"
#include "firebase/admob/rewarded_video.h"
#include <android/native_activity.h>
#include "../function/GameFunction.h"

static bool ProcessEvents(int msec)
{
    sf::Time _msec = sf::milliseconds(msec);
    sf::sleep(_msec);
    return true;
}

/// check ad object state
static bool checkAdState(firebase::FutureBase future)
{
    return (future.status() == firebase::kFutureStatusComplete &&
            future.error() == firebase::admob::kAdMobErrorNone);
}

static void WaitForFutureCompletion(firebase::FutureBase future)
{
    while (!ProcessEvents(1000))
    {
        if (future.status() != firebase::kFutureStatusPending) break;
    }
    if (future.error() != firebase::admob::kAdMobErrorNone)
    {
        is::showLog("ERROR: Action failed with error code " + is::numToStr(future.error()) +
                    " and message : " + future.error_message());
    }
}

class AdmobManager
{
private:
    // The AdMob components IDs.
    const char* kAdMobAppID          = "ca-app-pub-3940256099942544~3347511713";
    const char* kBannerAdUnit        = "ca-app-pub-3940256099942544/6300978111";
    const char* kRewardedVideoAdUnit = "ca-app-pub-3940256099942544/2888167318";

    // Standard mobile banner size is 320x50.
    static const int kBannerWidth = 640;
    static const int kBannerHeight = 48;

    // Sample birthday value to use in making the request.
    static const int kBirthdayDay = 1;
    static const int kBirthdayMonth = 1;
    static const int kBirthdayYear = 1990;

public:
    firebase::App* m_admobApp;
    firebase::admob::AdRequest request;
    firebase::admob::BannerView* banner;
    ANativeActivity* m_activity;
    sf::RenderWindow &m_app;
    GameSystemExtended &m_gameSysExt;
    bool m_changeBannerPos;
    bool m_showBanner;
    bool m_showRewardVideo;

    ~AdmobManager()
    {
        delete banner;
        firebase::admob::rewarded_video::Destroy();
        firebase::admob::Terminate();
        delete m_admobApp;
    }

    AdmobManager(sf::RenderTexture &surface, GameSystemExtended &gameSystemExt, ANativeActivity* activity, JNIEnv* env, JavaVM* vm) :
            m_activity(activity),
            m_app(app),
            m_gameSysExt(gameSystemExt),
            m_changeBannerPos(false),
            m_showBanner(false),
            m_showRewardVideo(false)
    {
        m_admobApp = ::firebase::App::Create(firebase::AppOptions(), env, m_activity->clazz);
        firebase::admob::Initialize(*m_admobApp, kAdMobAppID);

        // If the app is aware of the user's gender, it can be added to the targeting
        // information. Otherwise, "unknown" should be used.
        request.gender = firebase::admob::kGenderUnknown;

        // This value allows publishers to specify whether they would like the request
        // to be treated as child-directed for purposes of the Children’s Online
        // Privacy Protection Act (COPPA).
        // See http://business.ftc.gov/privacy-and-security/childrens-privacy.
        request.tagged_for_child_directed_treatment = firebase::admob::kChildDirectedTreatmentStateTagged;

        // The user's birthday, if known. Note that months are indexed from one.
        request.birthday_day = kBirthdayDay;
        request.birthday_month = kBirthdayMonth;
        request.birthday_year = kBirthdayYear;

        // Sample keywords to use in making the request.
        static const char* kKeywords[] = {"Football", "Game", "Music", "Applications"};

        // Sample test device IDs to use in making the request.
        static const char* kTestDeviceIDs[] = {"2077ef9a63d2b398840261c8221a0c9b",
                                               "098fe087d987c9a878965454a65654d7"};

        // Additional keywords to be used in targeting.
        request.keyword_count = sizeof(kKeywords) / sizeof(kKeywords[0]);
        request.keywords = kKeywords;

        // "Extra" key value pairs can be added to the request as well. Typically
        // these are used when testing new features.
        static const firebase::admob::KeyValuePair kRequestExtras[] = {{"the_name_of_an_extra", "the_value_for_that_extra"}};
        request.extras_count = sizeof(kRequestExtras) / sizeof(kRequestExtras[0]);
        request.extras = kRequestExtras;

        // This example uses ad units that are specially configured to return test ads
        // for every request. When using your own ad unit IDs, however, it's important
        // to register the device IDs associated with any devices that will be used to
        // test the app. This ensures that regardless of the ad unit ID, those
        // devices will always m_receive test ads in compliance with AdMob policy.
        //
        // Device IDs can be obtained by checking the logcat or the Xcode log while
        // debugging. They appear as a long string of hex characters.
        request.test_device_id_count = sizeof(kTestDeviceIDs) / sizeof(kTestDeviceIDs[0]);
        request.test_device_ids = kTestDeviceIDs;

        // Create an ad size for the BannerView.
        firebase::admob::AdSize banner_ad_size;
        banner_ad_size.ad_size_type = firebase::admob::kAdSizeStandard;
        banner_ad_size.width = kBannerWidth;
        banner_ad_size.height = kBannerHeight;

        banner = new firebase::admob::BannerView();
        banner->Initialize(m_activity->clazz, kBannerAdUnit, banner_ad_size);

        firebase::admob::rewarded_video::Initialize();
    }

    void loadBannerAd()
    {
        if (checkAdState(banner->InitializeLastResult()))
        {
            banner->LoadAd(request);
        }
    };

    void showBannerAd()
    {
        if (checkAdState(banner->LoadAdLastResult()))
        {
            if (!checkAdState(banner->ShowLastResult()) || checkAdState(banner->HideLastResult()))
            {
                if (!m_showBanner)
                {
                    if (!m_changeBannerPos)
                    {
                        m_changeBannerPos = true;
                    }
                    banner->Show();
                    WaitForFutureCompletion(banner->ShowLastResult());
                    m_showBanner = true;
                }
            }
        }
    };

    void hideBannerAd()
    {
        if (checkAdState(banner->LoadAdLastResult()))
        {
            m_showBanner = false;
            banner->Hide();
        }
    };

    void loadRewardVideo()
    {
        if (checkAdState(firebase::admob::rewarded_video::InitializeLastResult()))
        {
            firebase::admob::rewarded_video::LoadAd(kRewardedVideoAdUnit, request);
        }
    };

    auto updateSFMLApp(bool whiteColor)
    {
        sf::Event m_event;
        while (m_app.pollEvent(m_event));
        m_app.clear((whiteColor) ? sf::Color::White : sf::Color::Black);
        m_app.display();
    };

    void checkAdObjInit()
    {
        while (!checkAdState(banner->InitializeLastResult()) &&
                !checkAdState(firebase::admob::rewarded_video::InitializeLastResult())
                )
        {
            updateSFMLApp(true);
        }
    }

    void checkAdRewardObjReinitialize()
    {
        firebase::admob::rewarded_video::Destroy();
        firebase::admob::rewarded_video::Initialize();
        while (!checkAdState(firebase::admob::rewarded_video::InitializeLastResult())) updateSFMLApp(true);
        loadRewardVideo();
    }
};
#endif // defined
#endif // defined
#endif // ADMOBMANAGER_H_INCLUDED
