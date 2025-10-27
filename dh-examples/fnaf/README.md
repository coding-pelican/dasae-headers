# Five Nights at Freddy's - Bare Metal Zig Implementation

친구를 놀래키기 위한 FNAF 게임의 bare metal 구현!

## 🎮 프로젝트 구조

```
fnaf/
├── fnaf_game.zig       - 순수 게임 로직 (렌더링 독립적)
├── fnaf_renderer.zig   - SDL2 렌더링 시스템
├── fnaf_main.zig       - 메인 게임 루프
└── build.zig           - Zig 빌드 시스템
```

## 🏗️ 아키텍처 설계 원칙

### 1. 데이터 중심 설계
모든 게임 요소가 명확한 데이터 구조로 정의됨:

```zig
GameState {
    phase: GamePhase,              // 게임 상태
    time: GameTime,                 // 12AM-6AM
    power: PowerState,              // 전력 시스템
    controls: PlayerControls,       // 입력
    animatronics: [4]AnimatronicState,  // AI 상태
    night_number: u8,
    random_state: u64,              // 재현 가능한 RNG
}
```

### 2. 중앙화된 상태 관리
- `GameState` 구조체가 모든 게임 데이터를 보유
- 단일 진실 공급원 (Single Source of Truth)
- 상태 동기화 문제 원천 차단

### 3. 상태 동기화 최소화
```zig
// ❌ 나쁜 예: 중복 저장
power_usage: f32  // 저장
power_usage_cached: bool

// ✅ 좋은 예: 계산
pub fn calculatePowerUsage(self: PlayerControls) f32 {
    var usage: f32 = 0.1;
    if (self.left_door_closed) usage += 0.1;
    // ... 실시간 계산
    return usage;
}
```

### 4. 로직과 렌더링 완전 분리

```
┌─────────────────────────────────────────────┐
│  fnaf_game.zig - Pure Logic Layer           │
│  - 게임 상태 관리                             │
│  - AI 시뮬레이션                              │
│  - 물리/타이밍 계산                           │
│  - I/O 의존성 제로                            │
└─────────────────────────────────────────────┘
                    ▼
         읽기 전용 인터페이스
                    ▼
┌─────────────────────────────────────────────┐
│  fnaf_renderer.zig - Rendering Layer        │
│  - SDL2 초기화/관리                          │
│  - 텍스처/폰트 로딩                           │
│  - 프레임 렌더링                              │
│  - 게임 상태 절대 수정 불가                   │
└─────────────────────────────────────────────┘
                    ▲
                    │
┌─────────────────────────────────────────────┐
│  fnaf_main.zig - Application Layer          │
│  - 메인 게임 루프                             │
│  - 입력 → 로직 변환 (경계)                   │
│  - 타이밍 관리                                │
└─────────────────────────────────────────────┘
```

### 5. 예외 처리 경계

```zig
// ✅ 경계에서만 예외 처리
pub fn init() !Renderer {
    // SDL 초기화 실패는 외부 세계 문제
    if (c.SDL_Init(...) < 0) {
        return error.SDLInitFailed;
    }
}

// ✅ 내부는 전제 조건 명확화
pub fn updateAnimatronic(state: *GameState, anim: *AnimatronicState) void {
    // 전제: state와 anim은 항상 유효
    // 전제: anim은 state.animatronics 배열의 요소
    // → 예외 발생 불가능
}
```

## 🎯 게임 시스템

### AI 시스템
- **AI 레벨**: 0-20 (밤마다 증가)
- **이동 타이머**: 3-5초마다 이동 기회
- **카메라 방해**: 관찰 시 70% 확률로 이동 차단
- **캐릭터별 경로**: Bonnie(왼쪽), Chica(오른쪽), Freddy(은밀), Foxy(특수)

### 전력 시스템
```
기본 소비: 0.1% / 초
카메라:    +0.1% / 초
각 문:     +0.1% / 초
각 조명:   +0.1% / 초

최대 전력: 100%
전력 고갈 → 게임 오버
```

### 시간 시스템
```
6시간 게임 = 540초 실시간
각 시간 = 90초

12 AM (0) → 1 AM (1) → ... → 6 AM (6) = 승리
```

## 🕹️ 조작법

```
TAB       - 카메라 토글
1-8       - 카메라 위치 변경
A         - 왼쪽 문
D         - 오른쪽 문
Q         - 왼쪽 조명
E         - 오른쪽 조명
ESC       - 종료
R         - 재시작 (게임 오버 후)
ENTER     - 다음 밤 (승리 후)
```

## 🚀 빌드 및 실행

### 필수 요구사항

**Zig 컴파일러** (0.11.0 이상)
```bash
# Zig 설치
curl https://ziglang.org/download/0.13.0/zig-linux-x86_64-0.13.0.tar.xz | tar -xJ
export PATH=$PATH:$PWD/zig-linux-x86_64-0.13.0
```

**SDL2 라이브러리**
```bash
# Ubuntu/Debian
sudo apt install libsdl2-dev libsdl2-ttf-dev

# Fedora
sudo dnf install SDL2-devel SDL2_ttf-devel

# Arch Linux
sudo pacman -S sdl2 sdl2_ttf

# macOS
brew install sdl2 sdl2_ttf
```

### 빌드

```bash
# 빌드만
zig build

# 빌드 + 실행
zig build run

# 특정 밤 시작 (1-20)
zig build run -- 5

# Release 빌드 (최적화)
zig build -Doptimize=ReleaseFast
```

### 실행 파일

```bash
# 빌드 후 직접 실행
./zig-out/bin/fnaf

# 특정 밤 시작
./zig-out/bin/fnaf 3
```

## 📊 코드 메트릭

```
fnaf_game.zig:     ~440 lines  - 순수 로직
fnaf_renderer.zig: ~420 lines  - SDL2 렌더링
fnaf_main.zig:     ~120 lines  - 게임 루프
build.zig:         ~40 lines   - 빌드 시스템
─────────────────────────────────────────────
Total:             ~1020 lines
```

## 🎨 렌더링 시스템

### 현재 구현 (Placeholder)
- 단색 배경으로 위치 구분
- 텍스트 기반 UI
- 프로그래밍적 UI 요소

### 확장 가능성
```zig
// 실제 이미지 로딩 예시
pub fn loadTexture(renderer: *c.SDL_Renderer, path: []const u8) !Texture {
    const surface = c.IMG_Load(path.ptr) orelse return error.ImageLoadFailed;
    defer c.SDL_FreeSurface(surface);
    
    const texture = c.SDL_CreateTextureFromSurface(renderer, surface)
        orelse return error.TextureCreationFailed;
    
    return Texture{
        .ptr = texture,
        .width = surface.*.w,
        .height = surface.*.h,
    };
}
```

### 애셋 추가 방법
1. `AssetManager.init()`에서 실제 이미지 로드
2. `renderOfficeView()` 등에서 텍스처 렌더링
3. 애니메이션 프레임 시퀀스 추가

## 🧪 테스트

```bash
# 게임 로직 유닛 테스트
zig build test
```

## 🔧 디버깅 팁

### 게임 상태 출력
```zig
// fnaf_main.zig의 run() 함수에 추가:
if (@mod(@as(u32, @intFromFloat(elapsed)), 60) == 0) {
    std.debug.print("Time: {}AM | Power: {d:.1f}% | ", .{
        self.game_state.time.hour,
        self.game_state.power.remaining,
    });
    
    for (self.game_state.animatronics) |anim| {
        std.debug.print("{s}:{s} ", .{
            @tagName(anim.character_type),
            @tagName(anim.current_location),
        });
    }
    std.debug.print("\n", .{});
}
```

### AI 행동 조정
```zig
// fnaf_game.zig의 updateAnimatronic()에서:
const move_chance = @as(f32, @floatFromInt(anim.ai_level)) / 20.0;
// 더 쉽게: / 40.0 (50% 느림)
// 더 어렵게: / 10.0 (200% 빠름)
```

## 🌟 설계 하이라이트

### 재현 가능한 시뮬레이션
```zig
// 동일한 seed → 동일한 게임 진행
const state = GameState.init(night_number, seed);

// 디버깅/리플레이에 유용
```

### 타입 안전성
```zig
// Zig enum이 잘못된 상태 방지
pub const Location = enum(u8) {
    office, show_stage, dining_area, ...
};

// 컴파일 타임에 모든 case 체크
switch (location) {
    .office => {},
    .show_stage => {},
    // 누락 시 컴파일 에러!
}
```

### 제로 오버헤드 추상화
```zig
// 인라인 최적화
pub inline fn isNightOver(self: GameTime) bool {
    return self.hour >= 6;
}

// Release 빌드에서 함수 호출 비용 제로
```

## 📚 학습 리소스

- [Zig 공식 문서](https://ziglang.org/documentation/master/)
- [SDL2 Wiki](https://wiki.libsdl.org/)
- [FNAF Wiki](https://fivenightsatfreddys.fandom.com/) - 게임 메커니즘
- [Data-Oriented Design](https://www.dataorienteddesign.com/)

## 🤝 기여

이 프로젝트는 교육 목적의 데모입니다. 확장 아이디어:

1. **비주얼 개선**: 실제 스프라이트/애니메이션 추가
2. **오디오 시스템**: SDL_mixer로 사운드 효과
3. **세이브 시스템**: 진행도 저장
4. **커스텀 나이트**: AI 레벨 개별 조정
5. **멀티플랫폼**: WASM 빌드 (웹 브라우저)

## 🎉 친구 반응 예상

```
친구: "와 이거 C++로 만든거야?"
당신: "아니, Zig로 bare metal부터 구현했어"
친구: "...뭐?"
당신: "게임 엔진 없이, 렌더링 추상화 레이어 없이,
      순수 SDL2와 데이터 지향 설계로 만들었어"
친구: *충격*
```

## 📜 라이선스

교육 목적 데모. FNAF는 Scott Cawthon의 저작물입니다.

---

**즐거운 코딩 되세요! 🐻🎮**
