#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>
#include <QGSP_BERT.hh>
#include <G4GenericAnalysisManager.hh>
#include "muic/actions/RunAction.h"
#include "muic/setup/ActionInitialization.h"
#include "muic/setup/DetectorConstruction.h"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "Randomize.hh"
#include "muic/setup/PhysicsList.h"

boost::program_options::variables_map parse_options(int argc, char **argv) {
    using namespace boost::program_options;

    variables_map args;

    try {
        options_description opt_desc("Options");
        opt_desc.add_options()
                (
                        "help,h",
                        "Display help"
                )
                (
                        "interactive,i",
                        value<bool>()
                                ->value_name("INTERACTIVE_EN")
                                ->default_value(false, "FALSE"),
                        "If enabled, the ui interface is displayed."
                )
                (
                        "exec,x",
                        value<std::string>()
                                ->value_name("EXECUTE_CMD")
                                ->default_value("", "BLANK"),
                        "If given, this will be run on start-up. When no command is provided this will enable interactive mode."
                )
                (
                        "ui-verbosity,",
                        value<unsigned int>()
                                ->value_name("UI_VERBOSITY")
                                ->default_value(0),
                        "Verbosity level for user interface."
                )
                (
                        "pl-verbosity,",
                        value<unsigned int>()
                                ->value_name("PHYSICS_LIST_VERBOSITY")
                                ->default_value(0),
                        "Verbosity level for physics list."
                );

        positional_options_description pos_desc;

        command_line_parser parser(argc, argv);
        parser.options(opt_desc)
                .positional(pos_desc)
                .allow_unregistered();

        auto parsed_options = parser.run();

        store(parsed_options, args);
        notify(args);

        if (args.count("help")) {
            std::cout << opt_desc << std::endl;
            exit(0);
        }
    } catch (const error &ex) {
        std::cerr << ex.what() << std::endl;
    }

    return args;
}

int main(int argc, char **argv) {
    // Parse Args
    auto cmd_args = parse_options(argc, argv);

    auto interactive_en = cmd_args["interactive"].as<bool>();
    auto execute_cmd = cmd_args["exec"].as<std::string>();
    auto ui_verbosity = cmd_args["ui-verbosity"].as<unsigned int>();
    auto pl_verbosity = cmd_args["pl-verbosity"].as<unsigned int>();

    // Enable UI if no macro is given
    if (execute_cmd == "") {
        G4cout << "No commands provided, enabling interactive mode."
               << G4endl;
        interactive_en = true;
    }

    // Load UI
    G4UIExecutive *ui = nullptr;

    if (interactive_en) {
        ui = new G4UIExecutive(argc, argv);
    }

    // Optionally: choose a different Random engine
    G4Random::setTheEngine(new CLHEP::RanecuEngine);

    // Construct the default run analysis_manager
    auto* run_manager = new G4MTRunManager;
    run_manager->SetNumberOfThreads(10);

    // Detector construction
    run_manager->SetUserInitialization(new DetectorConstruction());

    // Physics list
    auto *physics_list = new PhysicsList();
    physics_list->SetVerboseLevel(pl_verbosity);
    run_manager->SetUserInitialization(physics_list);

    // User action initialization
    run_manager->SetUserInitialization(new ActionInitialization());

    // Initialize visualization
    auto *vis_manager = new G4VisExecutive();
    vis_manager->SetVerboseLevel(ui_verbosity);
    vis_manager->Initialize();

    // Get the pointer to the User Interface analysis_manager
    auto *ui_manager = G4UImanager::GetUIpointer();

    // Process macro or start UI session
    if (interactive_en) {
        ui_manager->ApplyCommand("/control/execute macros/init_vis.mac");
        ui->SessionStart();
        delete ui;
    } else if (execute_cmd != "") {
        ui_manager->ApplyCommand(execute_cmd);
    }

    // Analysis Manager
    auto *analysis_manager = G4GenericAnalysisManager::Instance();
    analysis_manager->CloseFile();

    // Free resources
    delete vis_manager;
    delete run_manager;

    // Terminate with status code 0
    return 0;
}

