set( display_src "display/display.c"
                 "display/ili9341.c"
                 )

set(include_dirs "."
                 "display"
                 )

idf_component_register(SRCS "main.c"
                            "audio_module.c"
                            "user_input.c"
                            "system_message.c"
                            "${display_src}"
                        INCLUDE_DIRS "${include_dirs}"
                        )
