#pragma once
#include "Common.h"
#include "Singleton.h"

#define PARSE_FLAG_BOOL(name, offset) \
    [[nodiscard]] inline bool name() const { return (uiState() & (1 << offset)) != 0; }

struct LinkedMem;
struct MumbleContext;

enum class ConditionalState : u32
{
    None = 0,
    Underwater = 1,
    OnWater = 2,
    InCombat = 4,
    InWvW = 8,

    All = Underwater | OnWater | InCombat | InWvW,

    IsFlag
};

class MumbleLink : public Singleton<MumbleLink>
{
public:
    enum class Profession : u8
    {
        None = 0,
        Guardian = 1,
        Warrior = 2,
        Engineer = 3,
        Ranger = 4,
        Thief = 5,
        Elementalist = 6,
        Mesmer = 7,
        Necromancer = 8,
        Revenant = 9
    };

    enum class EliteSpec : u8
    {
        None = 0,
        Amalgam = 1,
        Antiquary = 2,
        Berserker = 3,
        Bladesworn = 4,
        Catalyst = 5,
        Chronomancer = 6,
        Conduit = 7,
        Daredevil = 8,
        Deadeye = 9,
        Dragonhunter = 10,
        Druid = 11,
        Evoker = 12,
        Firebrand = 13,
        Galeshot = 14,
        Harbinger = 15,
        Herald = 16,
        Holosmith = 17,
        Luminary = 18,
        Mechanist = 19,
        Mirage = 20,
        Paragon = 21,
        Reaper = 22,
        Renegade = 23,
        Ritualist = 24,
        Scourge = 25,
        Scrapper = 26,
        Soulbeast = 27,
        Specter = 28,
        Spellbreaker = 29,
        Tempest = 30,
        Troubadour = 31,
        Untamed = 32,
        Vindicator = 33,
        Virtuoso = 34,
        Weaver = 35,
        Willbender = 36,
    };

    enum class Race : u8
    {
        Asura = 0,
        Charr = 1,
        Human = 2,
        Norn = 3,
        Sylvari = 4
    };

    enum class MountType : u32
    {
        None = 0,
        Jackal,
        Griffon,
        Springer,
        Skimmer,
        Raptor,
        RollerBeetle,
        Warclaw,
        Skyscale,
        Skiff,
        SiegeTurtle
    };

    MumbleLink();
    ~MumbleLink() override;

    void OnUpdate();

    [[nodiscard]] bool isInWvW() const;

    // uiState flags
    PARSE_FLAG_BOOL(isMapOpen, 0);
    PARSE_FLAG_BOOL(isCompassTopRight, 1);
    PARSE_FLAG_BOOL(doesCompassHaveRotationEnabled, 2);
    PARSE_FLAG_BOOL(gameHasFocus, 3);
    PARSE_FLAG_BOOL(isInCompetitiveMode, 4);
    PARSE_FLAG_BOOL(textboxHasFocus, 5);
    PARSE_FLAG_BOOL(isInCombat, 6);

    [[nodiscard]] vec3 position() const;

    [[nodiscard]] MountType currentMount() const;
    [[nodiscard]] bool isMounted() const;
    [[nodiscard]] bool isInMap() const;
    [[nodiscard]] uint32_t mapId() const;
    [[nodiscard]] std::wstring characterName() const;
    [[nodiscard]] bool isSwimmingOnSurface() const;
    [[nodiscard]] bool isUnderwater() const;

    [[nodiscard]] bool isOnOrUnderwater() const { return isSwimmingOnSurface() || isUnderwater(); }

    [[nodiscard]] ConditionalState currentState() const {
        return (isSwimmingOnSurface() ? ConditionalState::OnWater : ConditionalState::None) |
               (isUnderwater() ? ConditionalState::Underwater : ConditionalState::None) |
               (isInCombat() ? ConditionalState::InCombat : ConditionalState::None) |
               (isInWvW() ? ConditionalState::InWvW : ConditionalState::None);
    }

    [[nodiscard]] Profession characterProfession() const { return identity_.profession; }

    [[nodiscard]] EliteSpec characterSpecialization() const;

    [[nodiscard]] Race characterRace() const { return identity_.race; }

    [[nodiscard]] bool isCommander() const { return identity_.commander; }

    [[nodiscard]] f32 fov() const { return identity_.fov; }

    [[nodiscard]] uint8_t uiScale() const { return identity_.uiScale; }

protected:
    std::wstring fileMappingName_ = L"MumbleLink";

    [[nodiscard]] uint32_t uiState() const;

    struct Identity
    {
        Profession profession = Profession::None;
        EliteSpec specialization = EliteSpec::None;
        Race race = Race::Asura;
        bool commander = false;
        f32 fov = 0.f;
        uint8_t uiScale = 0;
        std::string name;
    };

    [[nodiscard]] const MumbleContext* context() const;
    HANDLE fileMapping_ = nullptr;
    LinkedMem* linkedMemory_ = nullptr;

    Identity identity_;
};
