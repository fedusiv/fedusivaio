set( display_src "display/display.c"
                 "display/ili9341.c"
                 )

set( app_src "app/system_message.c"
            "app/app_manager.c"
                )

set( audio_src "audio/audio_hw.c"
            "audio/audio_module.c"
            "audio/sound_engine.c"
            "audio/sound_adsr.c"
                )
set( memory_src "memory/memory_manager.c"
                )

set(include_dirs "."
                 "display"
                 "app"
                 "memory"
                 )

idf_component_register(SRCS "${app_src}"
                            "${display_src}"
                            "${audio_src}"
                            "${memory_src}"
                            "user_input.c"
                            "main.c"
                        INCLUDE_DIRS "${include_dirs}"
                        )
