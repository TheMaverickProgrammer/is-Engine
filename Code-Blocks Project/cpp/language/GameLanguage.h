#ifndef GAMELANGUAGE_H_INCLUDED
#define GAMELANGUAGE_H_INCLUDED

/*
This file allows you to define the languages you want to use in the game.
To use a language you must define a array. the first index represents the first language
(e.g eng) and the index following its translation (e.g fr).

example to display several sentences in the dialog manager.
lang_array[] = {"eng 1", "translation fr 1", "eng 2", "translation fr 2", ...};
(Go to the GameDialog Class to see how we implement languages to make sentences in dialogs)
*/

namespace lang
{
// ------------------------ menu ------------------------
static std::string pad_main_menu[] = {"Main Menu", "Menu Principal"};

static std::wstring msg_erase_data[] = {L"Erase previous data and start\nnew game?",
                                        L"Effacer les données en cours \net démarrer un nouveau jeu?"};
static std::string pad_new_game[] = {"Start Game", "Nouvelle Partie"};
static std::string pad_continue_game[] = {"Continue Game", "Continuer le Jeu"};
static std::wstring pad_controller[] = {L"Controller", L"Contrôleur"};

static std::string opt_enable_sound[]    = {"Enable SOUND : YES",   "Activer SON : OUI"};
static std::string opt_disable_sound[]   = {"Enable SOUND : NO",    "Activer SON : NON"};
static std::string opt_enable_music[]    = {"Enable MUSIC : YES",   "Activer MUSIQUE : OUI"};
static std::string opt_disable_music[]   = {"Enable MUSIC : NO",    "Activer MUSIQUE : NON"};
static std::string opt_enable_vibrate[]  = {"Enable VIBRATE : YES", "Activer VIBREUR : OUI"};
static std::string opt_disable_vibrate[] = {"Enable VIBRATE : NO",  "Activer VIBREUR : NON"};
static std::string opt_game_lang[] = {"Game Language : ENGLISH",    "Langue du Jeu  : FRANCAIS"};

static std::string msg_quit_game[] = {"Quit game?", "Quitter le jeu?"};
static std::string msg_controller_opt_error[] = {"Only for Android!", "Uniquement pour Android!"};

static std::wstring msg_permute_AB[] = {L"Permute ", L"Permuter "};
static std::wstring msg_how_move_control[] = {L"Press a controller to change its position",
                                                 L"Appuyez sur un contrôleur pour changer sa position"};

// ------------------------ level dialog ------------------------
static std::string pad_dialog_skip[] = {"Skip", "Passer"};
static std::wstring dialog_player_move[] = {L"Press LEFT or RIGHT to move.\n"
                                           "Press A to Jump.",
                                                L"Appuie sur GAUCHE ou DROITE pour te déplacer.\n"
                                                "Appuie sur A pour sauter."};
static std::wstring dialog_player_attack[] = {L"Press B to launch an Electro Shock to destroy your enemy.",
                                                    L"Appuie sur B pour lancer un Electro Shock pour détruire\n"
                                                     "ton ennemi.",
                                               L"Press LEFT, RIGHT, UP, OR DOWN to change its direction.",
                                                    L"Appuie sur GAUCHE, DROITE, HAUT ou BAS pour changer\n"
                                                     "sa direction."};

// ------------------------ menu pause ------------------------
static std::string msg_pause_restart[] = {"Restart Game?\n(The current game will be lost)",
                                "Recommencer la partie?\n(La partie en cours sera perdue)"};
static std::string msg_pause_quit[] = {"Quit Game?\n(The current game will be lost)",
                                "Quitter la partie?\n(La partie en cours sera perdue)"};
static std::string pad_restart_game[] = {"Restart Game", "Recommencer"};
static std::string pad_quit_game[] = {"Quit Game", "Quitter le Jeu"};

// ------------------------ game over ------------------------
static std::wstring game_over[] = {L"Game Over", L"Partie Terminée"};

// ------------------------ game end ------------------------
static std::wstring end_msg_congrat[] = {L"Congratulation", L"Félicitation"};
static std::wstring end_msg_sentences[] = {L"\t\t  End of the Demo, Thanks for playing.\n",
                                                  L"\t\t   Fin de la Démo, Merci d’avoir joué."};
}

#endif // GAMELANGUAGE_H_INCLUDED
