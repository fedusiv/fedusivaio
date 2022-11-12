set( display_src "display/display.c"
                 "display/ili9341.c"
                 )

set( app_src "app/system_message.c"
            "app/app_manager.c"
                )
set(include_dirs "."
                 "display"
                 "app"
                 )

idf_component_register(SRCS "${app_src}"
                            "${display_src}"
                            "audio_module.c"
                            "user_input.c"
                            "main.c"
                        INCLUDE_DIRS "${include_dirs}"
                        )
