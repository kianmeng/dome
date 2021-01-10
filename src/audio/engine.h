typedef uint64_t CHANNEL_ID;
typedef struct {
  CHANNEL_ID id;
} AUDIO_CHANNEL_REF;

typedef enum {
  AUDIO_TYPE_UNKNOWN,
  AUDIO_TYPE_WAV,
  AUDIO_TYPE_OGG
} AUDIO_TYPE;

typedef enum {
  CHANNEL_INVALID,
  CHANNEL_INITIALIZE,
  CHANNEL_TO_PLAY,
  CHANNEL_DEVIRTUALIZE,
  CHANNEL_LOADING,
  CHANNEL_PLAYING,
  CHANNEL_STOPPING,
  CHANNEL_STOPPED,
  CHANNEL_VIRTUALIZING,
  CHANNEL_VIRTUAL,
  CHANNEL_LAST
} CHANNEL_STATE;

struct CHANNEL_t;
typedef void (*CHANNEL_mix)(struct CHANNEL_t* channel, float* buffer, size_t requestedSamples);
typedef void (*CHANNEL_callback)(WrenVM* vm, struct CHANNEL_t* channel);

typedef struct CHANNEL_t {
  CHANNEL_STATE state;
  CHANNEL_ID id;
  volatile bool enabled;
  bool stopRequested;
  CHANNEL_mix mix;
  CHANNEL_callback update;
  CHANNEL_callback finish;

  void* userdata;
} CHANNEL;

typedef struct {
  SDL_AudioSpec spec;
  AUDIO_TYPE audioType;
  // Length is the number of LR samples
  uint32_t length;
  // Audio is stored as a stream of interleaved normalised values from [-1, 1)
  float* buffer;
} AUDIO_DATA;

struct AUDIO_CHANNEL_PROPS {
  // Control variables
  bool loop;
  // Playback variables
  float volume;
  float pan;
  // Position is the sample value to play next
  volatile size_t position;
  bool resetPosition;
};

typedef struct {
  struct AUDIO_CHANNEL_PROPS current;
  struct AUDIO_CHANNEL_PROPS new;
  char* soundId;
  float actualVolume;
  float actualPan;
  bool fade;

  AUDIO_DATA* audio;
  WrenHandle* audioHandle;
} AUDIO_CHANNEL;

internal void AUDIO_CHANNEL_mix(CHANNEL* base, float* stream, size_t totalSamples);
internal void AUDIO_CHANNEL_update(WrenVM* vm, CHANNEL* base);
internal void AUDIO_CHANNEL_finish(WrenVM* vm, CHANNEL* base);
internal void CHANNEL_requestStop(CHANNEL* channel);