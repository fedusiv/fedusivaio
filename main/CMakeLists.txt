set( display_src "display/display.c"
                 "display/ili9341.c"
                 )

set( app_src "app/system_message.c"
                )
set(include_dirs "."
                 "display"
                 "app"
                 )

idf_component_register(SRCS "main.c"
                            "audio_module.c"
                            "user_input.c"
                            "${display_src}"
                            "${app_src}"
                        INCLUDE_DIRS "${include_dirs}"
                        )
