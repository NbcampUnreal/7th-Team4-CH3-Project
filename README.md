# F4 — 덕코프 오마주 TPS

> 내일배움캠프 팀 프로젝트 | Unreal Engine 5 C++ 싱글플레이어 TPS | 2026.02.05 ~ 2026.03.05

[![시연 영상](https://img.shields.io/badge/YouTube-시연영상-red?logo=youtube)](https://www.youtube.com/watch?v=7bioMqeDRfw)
[![발표 자료](https://img.shields.io/badge/Google%20Slides-발표자료-yellow?logo=google)](https://docs.google.com/presentation/d/1HMPHXYiVAFUqABqqbM5wxkKn8VwhJEK-b67LwbfZPdk/edit?usp=sharing)
[![다운로드](https://img.shields.io/badge/Google%20Drive-배포파일(Shipping)-blue?logo=googledrive)](https://drive.google.com/file/d/1ozYzqGW6tglA0cLxiQgToMYBROqGNB4v/view?usp=sharing)

덕코프에서 영감을 받은 3인칭 슈터 게임입니다.

GAS(Gameplay Ability System) 기반 전투, Fragment 방식의 아이템 시스템, BehaviorTree AI를 직접 구현하며 UE5 게임 개발 구조를 학습했습니다.

---

## 목차

- [게임 플레이](#게임-플레이)
- [주요 기능](#주요-기능)
- [아키텍처 개요](#아키텍처-개요)
- [기술 스택](#기술-스택)
- [프로젝트 구조](#프로젝트-구조)
- [팀원 및 역할](#팀원-및-역할)

---

## 게임 플레이

| 기능 | 설명 |
|------|------|
| 이동 / 회피 | 달리기, 점프, 구르기 |
| 전투 | 조준, 사격, 재장전, 무기 전환, 특수 스킬(허리케인 킥) |
| 인벤토리 | 아이템 습득 · 장착 · 소비 · 드롭 |
| 퀵슬롯 | 무기 2슬롯(1-2) + 소비 아이템 6슬롯(3-8) |
| 적 AI | BehaviorTree 기반 근접 / 원거리 / 보스 패턴 |
| 동적 환경 | 주야간 사이클, 날씨 시스템 |
| 세션 지속 | 생존 후 귀환 시 인벤토리 · 난이도 데이터 유지 |

---

## 주요 기능

### GAS(Gameplay Ability System) 기반 전투
- 모든 행동(사격, 재장전, 구르기 등)을 **GameplayAbility**로 구현
- 플레이어 / 적 / 무기 AttributeSet 분리 (HP, Stamina, Ammo 등)
- `ModMagnitudeCalculation`으로 데미지 · 체력 · 탄약 계산 공식 분리
- 무적 포션 등 버프를 GameplayEffect로 처리

### Fragment 기반 아이템 시스템
- **ItemDefinition**(에셋) + **ItemInstance**(런타임) 구조
- Fragment를 조합해 아이템 기능을 정의
  - `Equipment` · `Firearm` · `Consumable` · `Stackable` · `UI` 등
- 총기 · 소비 아이템 · 탄약 · 픽업을 동일한 인터페이스로 처리

### 무기 / 퀵슬롯 분리 아키텍처
- **EquipmentComponent** : Primary / Secondary 무기 슬롯 전담 (Source of Truth)
- **QuickSlotComponent** : 소비 아이템 슬롯 0–5 전담
- UI도 분리 : `F4WeaponSlotWidget` vs `F4QuickSlotWidget`
- 입력도 분리 : 키 1·2 → 무기 전환(GAS Event), 키 3–8 → 소비 아이템 사용

### 세션 간 데이터 지속 (GameInstance)
- 생존 귀환 시 인벤토리 · 장비 · 퀵슬롯 상태 저장 후 복원
- 생존 횟수에 따른 영구 난이도 증가
- 사망 시 데이터 초기화

### 동적 난이도 & 적 스폰
- `F4GameState` Tick으로 생존 시간 누적 → 난이도 페이즈 자동 증가
- `EnemySpawner` : 난이도·거리·지형을 고려한 LineTrace 기반 스폰
- 야간에만 보스 스폰

### 동적 환경 시스템
- `F4GameState` 시간 시스템과 `DynamicSky`가 연동
- 태양 / 달 / 하늘 / 안개를 실시간 보간
- `WeatherData` 애셋으로 낮밤별 조명 설정 교체

### 무기 타입별 애니메이션
- `F4BaseAnimInst` 상속 계층으로 무기 타입별 클래스 분리
  - 맨손 · 근접 · 권총 · 소총
- AnimNotify로 사운드 · 이펙트 · GAS 이벤트 연결

### Enhanced Input + GameplayTag 기반 입력 처리
**InputConfig 에셋**에 `InputAction → GameplayTag` 매핑을 두 그룹으로 분리하여 정의

**네이티브 입력 (NativeInputActions)**
- 이동·카메라·줌처럼 매 프레임 수치를 넘겨야 하는 입력을 별도 관리
- `BindNativeAction(InputConfig, InputTag, ...)` 로 태그에 해당하는 InputAction을 조회해 C++ 콜백에 직접 바인딩

**어빌리티 입력 (AbilityInputActions)**
- `BindAbilityActions`가 AbilityInputActions 목록 전체를 순회하며 InputTag를 인자로 `AbilityInputTagPressed` / `AbilityInputReleased` 에 바인딩
- 입력 발생 시 ActivatableAbilities를 순회해 해당 InputTag를 AssetTag 또는 DynamicSpecSourceTag로 가진 AbilitySpec 핸들을 큐(InputPressedSpecHandles)에 적재
- 매 Tick `ProcessAbilityInput()`이 큐를 소비하며 `TryActivateAbility(Handle)` 호출
- 예) 구르기 InputAction → `Ability.Movement.Roll` 태그 → 해당 태그가 달린 GA_Roll 자동 활성화
- 입력 코드와 어빌리티 코드 간 직접 의존성 없이 **태그만으로 느슨하게 연결**

### BehaviorTree 기반 적 AI
- 타겟 감지 서비스 / 공격 태스크 / 위치 탐색 태스크
- 근접 · 원거리 · 보스 스킬을 GameplayAbility로 구현

---

## 아키텍처 개요

```
F4GameInstance  ← 세션 간 데이터(인벤토리, 난이도) 보존
│
├── LobbyGameMode ─────────────────── 맵 전환, BGM
└── BattleGameMode ─────────────────── 난이도 페이즈, 사망/귀환 처리
    │
    ├── F4GameState  ← 생존 시간 Tick, 주야간 사이클
    │   └── DynamicSky  ← 조명·대기 실시간 보간
    │
    ├── AF4PlayerCharacter
    │   ├── F4AbilitySystemComponent  ← GAS 핵심
    │   │   ├── AttributeSet (Player / Weapon)
    │   │   └── GameplayAbilities (20종)
    │   ├── F4EquipmentComponent      ← 무기 슬롯 (Primary / Secondary)
    │   ├── F4QuickSlotComponent      ← 소비 아이템 슬롯 0–5
    │   ├── F4InventoryComponent
    │   └── F4AnimControlComponent
    │
    ├── AF4EnemyBase
    │   ├── F4AbilitySystemComponent
    │   └── F4BTEnemyController       ← BehaviorTree AI
    │
    └── F4HUD
        ├── StatBarWidget (HP / Stamina)
        ├── F4QuickSlotBarWidget
        │   ├── F4WeaponSlotWidget × 2
        │   └── F4QuickSlotWidget × 6
        ├── F4InventoryWidget
        │   └── F4InventoryContextMenu
        ├── PauseMenuWidget
        └── DeathMenuWidget
```

### 아이템 데이터 흐름

```
UF4ItemDefinition (에셋)
  └── Fragments (기능 컴포넌트 조합)
        ├── Fragment_Equipment   → EquipmentComponent
        ├── Fragment_Firearm     → WeaponActor
        ├── Fragment_Consumable  → QuickSlotComponent
        └── Fragment_UI          → SlotWidget
```

---

## 기술 스택

| 항목 | 내용 |
|------|------|
| 엔진 | Unreal Engine 5.6 |
| 언어 | C++ |
| 능력 시스템 | Gameplay Ability System (GAS) |
| 입력 | Enhanced Input |
| AI | Behavior Tree |
| UI | UMG (C++ 기반) |
| 파티클 | Niagara |
| 버전 관리 | Git / GitHub |

---

## 프로젝트 구조

```
Source/F4/
├── AbilitySystem/
│   ├── Abilities/
│   │   ├── Combat/       # 사격, 재장전, 조준, 무기전환, 특수스킬, 사망
│   │   ├── Enemy/        # 근접·원거리·보스 공격, 적 사망
│   │   ├── Movement/     # 점프, 달리기, 구르기
│   │   ├── Interaction/  # 장착, 줍기, 상호작용
│   │   └── Items/        # 소비 아이템, 포션, 무적 포션
│   ├── Attributes/       # AttributeSet (Player / Enemy / Weapon)
│   ├── Calculations/     # MMC (데미지·체력 계산)
│   └── Data/             # AbilitySet
├── Animation/
│   ├── AnimInst/         # Base / UnArmed / Melee / Pistol / Rifle
│   ├── Notifies/         # 사운드, 발소리, 이벤트, 스킬 이펙트
│   └── Data/             # 발소리 데이터
├── BehaviorTree/
│   ├── Task/             # 공격, 위치 탐색
│   ├── Service/          # 타겟 감지
│   └── Decorator/        # 능력 활성화 여부 체크
├── Characters/
│   ├── Base/             # CharacterBase
│   ├── Player/           # PlayerCharacter, PlayerController, CameraManager
│   └── Enemy/            # EnemyBase, EnemySpawner
├── Enviroment/           # DynamicSky, LevelMoveActor, WeatherData
├── Input/                # InputConfig, InputComponent (Enhanced Input)
├── Inventory/            # ItemDefinition, ItemInstance, Fragment들
│                         # InventoryComponent, EquipmentComponent, QuickSlotComponent
├── Items/
│   ├── Weapons/          # WeaponActor, Projectile, EnemyProjectile
│   └── ConsumableItems/  # ConsumableDataAsset, AmmoDataAsset
├── System/               # GameInstance, GameState, GameMode들, GameplayTags
└── UI/                   # HUD, 인벤토리, 퀵슬롯, 메뉴 위젯
```

---

## 팀원 및 역할

> 내일배움캠프 UE5 C++ 과정 — Team 4

| 이름  | 담당 영역                                                                                                                          |
|-----|--------------------------------------------------------------------------------------------------------------------------------|
| <nobr>[강두연 (팀장)](https://github.com/dooyeonk)</nobr> | 총기관련 어빌리티, 인벤토리, 장비, 퀵슬롯                                                                                                       |
| [권민성](https://github.com/ZodiacRay) | 캐릭터 및 움직임 관련 어빌리티, 스태미나, 발소리, HUD, UI, 환경(낮밤 시스템, BGM), 레벨 디자인, 사망 및 맵이동 등 게임 전반 흐름 시스템, Enhanced Input + GameplayTag 기반 입력 바인딩 |
| [김태우](https://github.com/xodn246) | 몬스터 AI, 난이도 시스템                                                                                                                |
| [박원종](https://github.com/WonJongPark) | 상호작용, 소모성 아이템 어빌리티, 버프 시스템, 근접무기 공격 어빌리티, 파티클, 사운드, 파밍 아이템 런타임 스폰(SpawnVolume)                                                 |

---

## 학습 포인트

이 프로젝트를 통해 학습한 주요 개념들:

- **GAS 아키텍처** : Ability · Attribute · Effect · Tag 의 연동 구조
- **컴포넌트 설계** : 책임 분리 원칙을 적용한 인벤토리 / 장비 / 퀵슬롯 분리
- **Fragment 패턴** : 상속 대신 조합(Composition)으로 아이템 기능 확장
- **AnimInst 계층** : 무기 타입별 애니메이션 클래스 분리 및 노티파이 연동
- **BehaviorTree AI** : Service · Task · Decorator 를 활용한 적 AI 구현
- **Enhanced Input + GameplayTag** : InputConfig 에셋으로 입력을 NativeInputActions(이동·카메라 직접 콜백)와 AbilityInputActions(GameplayTag 기반 어빌리티 활성화)로 분리, 입력 버퍼링 → ProcessAbilityInput Tick 처리로 느슨하게 연결
- **GameState 활용** : 시간 흐름 제어를 통한 환경 변화 및 난이도 관리
- **GameInstance 활용** : 레벨 전환 간 데이터 지속성 관리

---

<p align="center">내일배움캠프 UE5 C++ 과정 | Team 4</p>
