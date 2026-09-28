set(GAME_FILES
        src/game/board/audio.c
        src/game/board/basic_space.c
        src/game/board/battle.c
        src/game/board/block.c
        src/game/board/boo.c
        src/game/board/boo_house.c
        src/game/board/bowser.c
        src/game/board/char_wheel.c
        src/game/board/com.c
        src/game/board/com_path.c
        src/game/board/fortune.c
        src/game/board/item.c
        src/game/board/last5.c
        src/game/board/lottery.c
        src/game/board/main.c
        src/game/board/mg_setup.c
        src/game/board/model.c
        src/game/board/mushroom.c
        src/game/board/pause.c
        src/game/board/player.c
        src/game/board/roll.c
        src/game/board/shop.c
        src/game/board/space.c
        src/game/board/star.c
        src/game/board/start.c
        src/game/board/tutorial.c
        src/game/board/ui.c
        src/game/board/view.c
        src/game/board/warp.c
        src/game/board/window.c

        src/game/armem.c
        src/game/audio.c
        src/game/card.c
        src/game/chrman.c
        src/game/ClusterExec.c
        src/game/data.c
        src/game/decode.c
        src/game/dvd.c
        src/game/EnvelopeExec.c
        src/game/esprite.c
        src/game/fault.c
        src/game/flag.c
        src/game/font.c
        src/game/frand.c
        src/game/gamework.c
        src/game/hsfanim.c
        src/game/hsfdraw.c
        src/game/hsfex.c
        src/game/hsfload.c
        src/game/hsfman.c
        src/game/hsfmotion.c
        src/game/init.c
        src/game/main.c
        src/game/malloc.c
        src/game/mapspace.c
        src/game/memory.c
        src/game/messdata.c
        src/game/minigame_seq.c
        src/game/objdll.c
        src/game/objmain.c
        src/game/objsub.c
        src/game/objsysobj.c
        src/game/ovllist.c
        src/game/pad.c
        src/game/perf.c
        src/game/printfunc.c
        src/game/process.c
        src/game/saveload.c
        src/game/ShapeExec.c
        src/game/sprman.c
        src/game/sprput.c
        src/game/window.c
        src/game/wipe.c

        src/msm/msmfio.c
        src/msm/msmmem.c
        src/msm/msmmus.c
        src/msm/msmse.c
        src/msm/msmstream.c
        src/msm/msmsys.c

        src/libhu/setvf.c
        src/libhu/subvf.c
)

set(PORT_FILES
        src/port/achievements.cpp
        src/port/audio.cpp
        src/port/byteswap.cpp
        src/port/config.cpp
        src/port/dolassets.cpp
        #        src/port/dvd.c
        src/port/file_select.cpp
        src/port/file_select.hpp
        src/port/imgui.cpp
        src/port/io.cpp
        src/port/iso_validate.cpp
        src/port/msm.c
        src/port/OS.c
        src/port/portmain.cpp
        src/port/settings.cpp
        src/port/stubs.c
        src/port/version.cpp

        src/port/ui/achievements.cpp
        src/port/ui/achievements.hpp
        src/port/ui/bool_button.cpp
        src/port/ui/bool_button.hpp
        src/port/ui/button.cpp
        src/port/ui/button.hpp
        src/port/ui/component.cpp
        src/port/ui/component.hpp
        src/port/ui/controller_config.cpp
        src/port/ui/controller_config.hpp
        src/port/ui/document.cpp
        src/port/ui/document.hpp
        src/port/ui/event.cpp
        src/port/ui/event.hpp
        src/port/ui/graphics_tuner.cpp
        src/port/ui/graphics_tuner.hpp
        src/port/ui/input.cpp
        src/port/ui/input.hpp
        src/port/ui/modal.cpp
        src/port/ui/modal.hpp
        src/port/ui/nav_types.hpp
        src/port/ui/number_button.cpp
        src/port/ui/number_button.hpp
        src/port/ui/overlay.cpp
        src/port/ui/overlay.hpp
        src/port/ui/pane.cpp
        src/port/ui/pane.hpp
        src/port/ui/menu_bar.cpp
        src/port/ui/menu_bar.hpp
        src/port/ui/prelaunch.cpp
        src/port/ui/prelaunch.hpp
        src/port/ui/preset.cpp
        src/port/ui/preset.hpp
        src/port/ui/select_button.cpp
        src/port/ui/select_button.hpp
        src/port/ui/settings.cpp
        src/port/ui/settings.hpp
        src/port/ui/compat.cpp
        src/port/ui/string_button.cpp
        src/port/ui/string_button.hpp
        src/port/ui/tab_bar.cpp
        src/port/ui/tab_bar.hpp
        src/port/ui/ui.cpp
        src/port/ui/ui.hpp
        src/port/ui/window.cpp
        src/port/ui/window.hpp
)

set(REL_FILES
        src/REL/_minigameDLL/_minigameDLL.c

        src/REL/bootDll/language.c
        src/REL/bootDll/main.c

        src/REL/E3setupDLL/main.c
        src/REL/E3setupDLL/mgselect.c

        src/REL/instDll/font.c
        src/REL/instDll/main.c

        src/REL/m401Dll/main.c
        src/REL/m401Dll/main_ex.c

        src/REL/m402Dll/main.c

        src/REL/m403Dll/main.c
        src/REL/m403Dll/scene.c

        src/REL/m404Dll/main.c

        src/REL/m405Dll/main.c

        src/REL/m406Dll/main.c
        src/REL/m406Dll/map.c
        src/REL/m406Dll/player.c

        src/REL/m407dll/camera.c
        src/REL/m407dll/effect.c
        src/REL/m407dll/main.c
        src/REL/m407dll/map.c
        src/REL/m407dll/player.c
        src/REL/m407dll/score.c
        src/REL/m407dll/whomp.c
        src/REL/m407dll/whomp_score.c

        src/REL/m408Dll/camera.c
        src/REL/m408Dll/main.c
        src/REL/m408Dll/object.c
        src/REL/m408Dll/stage.c

        src/REL/m409Dll/cursor.c
        src/REL/m409Dll/main.c
        src/REL/m409Dll/player.c

        src/REL/m410Dll/game.c
        src/REL/m410Dll/main.c
        src/REL/m410Dll/player.c
        src/REL/m410Dll/stage.c

        src/REL/m411Dll/main.c

        src/REL/m412Dll/main.c

        src/REL/m413Dll/main.c

        src/REL/m414Dll/main.c

        src/REL/m415Dll/main.c
        src/REL/m415Dll/map.c

        src/REL/m416Dll/main.c
        src/REL/m416Dll/map.c

        src/REL/m417Dll/main.c
        src/REL/m417Dll/player.c
        src/REL/m417Dll/sequence.c
        src/REL/m417Dll/water.c

        src/REL/m418Dll/main.c
        src/REL/m418Dll/sequence.c

        src/REL/m419Dll/main.c

        src/REL/m420dll/camera.c
        src/REL/m420dll/main.c
        src/REL/m420dll/map.c
        src/REL/m420dll/player.c
        src/REL/m420dll/rand.c

        src/REL/m421Dll/main.c
        src/REL/m421Dll/map.c
        src/REL/m421Dll/player.c

        src/REL/m422Dll/main.c

        src/REL/m423Dll/main.c

        src/REL/m424Dll/ball.c
        src/REL/m424Dll/claw.c
        src/REL/m424Dll/main.c
        src/REL/m424Dll/map.c

        src/REL/m425Dll/main.c
        src/REL/m425Dll/thwomp.c

        src/REL/m426Dll/main.c

        src/REL/m427Dll/main.c
        src/REL/m427Dll/map.c
        src/REL/m427Dll/player.c

        src/REL/m428Dll/main.c
        src/REL/m428Dll/map.c
        src/REL/m428Dll/player.c

        src/REL/m429Dll/main.c

        src/REL/m430Dll/main.c
        src/REL/m430Dll/player.c
        src/REL/m430Dll/water.c

        src/REL/m431Dll/main.c
        src/REL/m431Dll/object.c

        src/REL/m432Dll/main.c

        src/REL/m433Dll/main.c
        src/REL/m433Dll/map.c
        src/REL/m433Dll/player.c

        src/REL/m434Dll/fish.c
        src/REL/m434Dll/main.c
        src/REL/m434Dll/map.c
        src/REL/m434Dll/player.c

        src/REL/m435Dll/main.c
        src/REL/m435Dll/sequence.c

        src/REL/m436Dll/main.c
        src/REL/m436Dll/sequence.c

        src/REL/m437Dll/main.c
        src/REL/m437Dll/sequence.c

        src/REL/m438Dll/fire.c
        src/REL/m438Dll/main.c
        src/REL/m438Dll/map.c

        src/REL/m439Dll/main.c

        src/REL/m440Dll/main.c
        src/REL/m440Dll/object.c

        src/REL/m441Dll/main.c

        src/REL/m442Dll/main.c
        src/REL/m442Dll/score.c

        src/REL/m443Dll/main.c
        src/REL/m443Dll/map.c
        src/REL/m443Dll/player.c

        src/REL/m444dll/datalist.c
        src/REL/m444dll/main.c
        src/REL/m444dll/pinball.c
        src/REL/m444dll/shadow.c

        src/REL/m445Dll/main.c

        src/REL/m446Dll/camera.c
        src/REL/m446Dll/card.c
        src/REL/m446Dll/cursor.c
        src/REL/m446Dll/deck.c
        src/REL/m446Dll/main.c
        src/REL/m446Dll/player.c
        src/REL/m446Dll/stage.c
        src/REL/m446Dll/table.c

        src/REL/m447dll/block.c
        src/REL/m447dll/camera.c
        src/REL/m447dll/main.c
        src/REL/m447dll/player.c
        src/REL/m447dll/player_col.c
        src/REL/m447dll/stage.c

        src/REL/m448Dll/main.c

        src/REL/m449Dll/main.c

        src/REL/m450Dll/main.c

        src/REL/m451Dll/m451.c

        src/REL/m453Dll/main.c
        src/REL/m453Dll/map.c
        src/REL/m453Dll/score.c

        src/REL/m455Dll/main.c
        src/REL/m455Dll/stage.c

        src/REL/m456Dll/main.c
        src/REL/m456Dll/stage.c

        src/REL/m457Dll/main.c

        src/REL/m458Dll/main.c

        src/REL/m459dll/main.c

        src/REL/m460Dll/main.c
        src/REL/m460Dll/map.c
        src/REL/m460Dll/player.c
        src/REL/m460Dll/score.c

        src/REL/m461Dll/main.c

        src/REL/m462Dll/main.c

        src/REL/m463Dll/main.c

        src/REL/mentDll/common.c
        src/REL/mentDll/main.c

        src/REL/messDll/main.c

        src/REL/mgmodedll/battle.c
        src/REL/mgmodedll/datalist.c
        src/REL/mgmodedll/free_play.c
        src/REL/mgmodedll/main.c
        src/REL/mgmodedll/mgmode.c
        src/REL/mgmodedll/minigame.c
        src/REL/mgmodedll/record.c
        src/REL/mgmodedll/tictactoe.c

        src/REL/modeltestDll/main.c
        src/REL/modeltestDll/modeltest00.c
        src/REL/modeltestDll/modeltest01.c

        src/REL/modeseldll/datalist.c
        src/REL/modeseldll/filesel.c
        src/REL/modeseldll/main.c
        src/REL/modeseldll/modesel.c

        src/REL/mpexDll/charsel.c
        src/REL/mpexDll/main.c
        src/REL/mpexDll/mgname.c
        src/REL/mpexDll/mpex.c

        src/REL/mstory2Dll/board_clear.c
        src/REL/mstory2Dll/board_entrance.c
        src/REL/mstory2Dll/board_miss.c
        src/REL/mstory2Dll/ending.c
        src/REL/mstory2Dll/main.c
        src/REL/mstory2Dll/mg_clear.c
        src/REL/mstory2Dll/mg_miss.c
        src/REL/mstory2Dll/save.c

        src/REL/mstory3Dll/main.c
        src/REL/mstory3Dll/result.c
        src/REL/mstory3Dll/result_seq.c
        src/REL/mstory3Dll/win_effect.c

        src/REL/mstory4Dll/main.c

        src/REL/mstoryDll/board_clear.c
        src/REL/mstoryDll/board_miss.c
        src/REL/mstoryDll/main.c
        src/REL/mstoryDll/mg_clear.c
        src/REL/mstoryDll/mg_miss.c
        src/REL/mstoryDll/save.c

        src/REL/option/camera.c
        src/REL/option/guide.c
        src/REL/option/record.c
        src/REL/option/room.c
        src/REL/option/rumble.c
        src/REL/option/scene.c
        src/REL/option/sound.c
        src/REL/option/state.c
        src/REL/option/window.c

        src/REL/present/camera.c
        src/REL/present/common.c
        src/REL/present/init.c
        src/REL/present/main.c
        src/REL/present/present.c

        src/REL/resultDll/battle.c
        src/REL/resultDll/datalist.c
        src/REL/resultDll/main.c

        src/REL/selmenuDll/main.c

        src/REL/staffDll/main.c

        src/REL/subchrselDll/main.c

        src/REL/w01Dll/main.c
        src/REL/w01Dll/mg_coin.c
        src/REL/w01Dll/mg_item.c

        src/REL/w02Dll/gamble.c
        src/REL/w02Dll/gendice.c
        src/REL/w02Dll/main.c
        src/REL/w02Dll/mg_coin.c
        src/REL/w02Dll/mg_item.c
        src/REL/w02Dll/roulette.c
        src/REL/w02Dll/shuffleboard.c

        src/REL/w03Dll/condor.c
        src/REL/w03Dll/main.c
        src/REL/w03Dll/mg_coin.c
        src/REL/w03Dll/mg_item.c
        src/REL/w03Dll/river.c
        src/REL/w03Dll/smoke.c
        src/REL/w03Dll/statue.c

        src/REL/w04Dll/big_boo.c
        src/REL/w04Dll/boo_event.c
        src/REL/w04Dll/bridge.c
        src/REL/w04Dll/main.c
        src/REL/w04Dll/mg_coin.c
        src/REL/w04Dll/mg_item.c

        src/REL/w05Dll/dolphin.c
        src/REL/w05Dll/hotel.c
        src/REL/w05Dll/main.c
        src/REL/w05Dll/mg_coin.c
        src/REL/w05Dll/mg_item.c
        src/REL/w05Dll/monkey.c

        src/REL/w06Dll/bowser.c
        src/REL/w06Dll/bridge.c
        src/REL/w06Dll/fire.c
        src/REL/w06Dll/main.c
        src/REL/w06Dll/mg_coin.c
        src/REL/w06Dll/mg_item.c

        src/REL/w10Dll/host.c
        src/REL/w10Dll/main.c
        src/REL/w10Dll/scene.c
        src/REL/w10Dll/tutorial.c

        src/REL/w20Dll/main.c

        src/REL/w21Dll/main.c

        src/REL/ztardll/font.c
        src/REL/ztardll/main.c
        src/REL/ztardll/select.c
)
