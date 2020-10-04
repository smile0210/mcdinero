//
//  McDineroTypes.h
//  McDinero
//
//  Created by Smile0210 on 02/03/15.
//
//

#ifndef McDinero_McDineroTypes_h
#define McDinero_McDineroTypes_h

#define characterCategory       0x000001
#define moneyCategory           0x000002
#define errorCategory           0x000003
#define crownCategory           0x000004
#define paupauCategory          0x000005

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    #define kBackgroundMusic            "audio/RapDinero.wav"
    #define kDineroSound                "audio/dinero.wav"
    #define kDineroMiniSound            "audio/dinero_mini.wav"
    #define kErrorSound                 "audio/error.wav"
    #define kErrorMiniSound             "audio/error_mini.wav"
    #define kEspanaAtacaSound           "audio/espanaataca.wav"
    #define kEspanaAtacaMiniSound       "audio/espanaataca_mini.v"
    #define kPauPauPauSound             "audio/paupaupau.wav"
    #define kPerfectoSound              "audio/perfectocorrecto.wav"
    #define kYiyiyiSound                "audio/yiyiyi.wav"
    #define kYuyuyuSound                "audio/yuyuyu.wav"
    #define kAsdfSound                  "audio/asdf.wav"
    #define kFalladoSound               "audio/hefallado2.wav"
    #define kPauPauPauSound2            "audio/paupaupau2.wav"
    #define kRapCompleteSound           "audio/dinerocomplete.wav"
    #define kAprendeSound               "audio/aprende.wav"
    #define kSoyEspanaSound             "audio/soyespana.wav"
    #define kMonarquiaSound             "audio/monarquia.wav"
    #define kAtaquesDinderoSound        "audio/ataquesdinero.wav"
#else
    #define kBackgroundMusic            "audio/RapDinero.mp3"
    #define kDineroSound                "audio/dinero.mp3"
    #define kDineroMiniSound            "audio/dinero_mini.mp3"
    #define kErrorSound                 "audio/error.mp3"
    #define kErrorMiniSound             "audio/error_mini.mp3"
    #define kEspanaAtacaSound           "audio/espanaataca.mp3"
    #define kEspanaAtacaMiniSound       "audio/espanaataca_mini.mp3"
    #define kPauPauPauSound             "audio/paupaupau.mp3"
    #define kPerfectoSound              "audio/perfectocorrecto.mp3"
    #define kYiyiyiSound                "audio/yiyiyi.mp3"
    #define kYuyuyuSound                "audio/yuyuyu.mp3"
    #define kAsdfSound                  "audio/asdf.mp3"
    #define kFalladoSound               "audio/hefallado2.mp3"
    #define kPauPauPauSound2            "audio/paupaupau2.mp3"
    #define kRapCompleteSound           "audio/dinerocomplete.mp3"
    #define kAprendeSound               "audio/aprende.mp3"
    #define kSoyEspanaSound             "audio/soyespana.mp3"
    #define kMonarquiaSound             "audio/monarquia.mp3"
    #define kAtaquesDinderoSound        "audio/ataquesdinero.mp3"
#endif

#define kCrownOriginalUnlocked      "ACrownOriginal"
#define kCrownBronzeUnlocked        "ACrownBronze"
#define kCrownSilverUnlocked        "ACrownSilver"
#define kCrownGoldUnlocked          "ACrownGold"
#define kErrorOriginalUnlocked      "AErrorOriginal"
#define kErrorBronzeUnlocked        "AErrorBronze"
#define kErrorSilverUnlocked        "AErrorSilver"
#define kErrorGoldUnlocked          "AErrorGold"
#define kPauPauGreenUnloked         "ABrevageGreen"
#define kPauPauRedUnloked           "ABrevageRed"
#define kPauPauYellowUnloked        "ABrevageYellow"
#define kPauPauPurpleUnloked        "ABrevagePurple"
#define kPauPauPinkUnloked          "ABrevagePink"
#define kPauPauOrangeUnloked        "ABrevageOrange"
#define kCentenarioUnlocked         "ACentenario"
#define kCharacterGoldUnlocked      "ACharacterGold"

#define kAPauPauGreenKey            "ABrevageGreenKey"
#define kAPauPauRedKey              "ABrevageRedKey"
#define kAPauPauYellowKey           "ABrevageYellowKey"
#define kAPauPauPurpleKey           "ABrevagePurpleKey"
#define kAPauPauPinkKey             "ABrevagePinkKey"
#define kAPauPauOrangeKey           "ABrevageOrangeKey"
#define kACentenarioKey             "ACentenarioKey"

enum CharacterDirection {
    Left,
    Right
};

enum CharacterEquipment {
    None,
    Glasses,
    Cloak,
    Crown
};

enum ActionType {
    MoveXActionType,
    MakeCashActionType,
    MakeErrorActionType,
    MakeCrownActionType,
    BlinkActionType,
    RapActionType,
    CheckActionType,
    MakePauPauActionType,
    PauPauActiveActionType,
    BlinkAndHydeActionType,
    ExitMiniatureActionType
};

enum MoneyType {
    Uno,
    Dos,
    Cinco,
    Diez,
    Veinte,
    Cincuenta,
    Cien,
    Doscientos,
    Quinientos,
    MoneyMax
};

enum NodeType {
    CashType,
    CrownType,
    ErrorType,
    CrownEmmiterType,
    PauPauNodeType,
    PauPauEmmiterType
};

enum PauPauType {
    PauPauAmarillo,     // Immunity
    PauPauRojo,         // Extra life or extra points
    PauPauVerde,        // Russian, double points
    PauPauNaranja,      // Magnet
    PauPauMorado,       // Invisible
    PauPauRosa,         // 
    PauPauMax
};

enum AchievementsType {
    CrownBronzeAchievementType,
    CrownSilverAchievementType,
    CrownGoldAchievementType,
    CrownDiamondAchievementType,
    ErrorBronzeAchievementType,
    ErrorSilverAchievementType,
    ErrorGoldAchievementType,
    ErrorDiamondAchievementType,
    PauPauGreenAchievementType,
    PauPauRedAchievementType,
    PauPauYellowAchievementType,
    PauPauPurpleAchievementType,
    PauPauPinkAchievementType,
    PauPauOrangeAchievementType,
    CentenarioAchievementType,
    CharacterAchievementType
};


#endif
