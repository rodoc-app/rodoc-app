# Project metadata.
project('rodoc', 'cpp',
  version : '1.0',
  default_options : ['warning_level=3', 'cpp_std=c++20'])

# Add JSONCpp as subproject.
jsoncpp = subproject('jsoncpp')
json_dep = dependency('jsoncpp')

# Which qt framework to use. Either 5 or 6.
qt_lib = 'qt6'

# Import Qt6 for using its tools.
qt = import(qt_lib)
# Add Qt6 as dependency.
qt_dep = dependency(qt_lib, modules: ['Core', 'Gui', 'Widgets'])

# All source files.
source = files('main.cpp',
	'core/family_tree.cpp',
	'core/family_tree_items.cpp',
	'core/config.cpp',
	'core/date.cpp',
	'core/file_parser.cpp',
	'core/strings.h',
	'core/person.cpp',
	'graphics/mainwindow.cpp',
	'graphics/mainwindow_slots.cpp',
	'graphics/dialogs.cpp',
	'graphics/graphics_items.cpp')

# Preprocess Qt files with its tools.
resources = qt.compile_resources(sources: ['graphics/resources.qrc'])

ui_files = qt.compile_ui(sources: ['graphics/mainwindow.ui',
				'graphics/date_dialog.ui',
				'graphics/tag_chooser.ui',
				'graphics/add_file.ui',
				'graphics/relation_templates.ui',
				'graphics/event_templates.ui',
				'graphics/help_dialog.ui',
				'graphics/choose_person.ui',
				'graphics/relation_dialog.ui',
				'graphics/visual_settings.ui',
				'graphics/export_templates.ui',
				'graphics/export_project.ui',
				'graphics/picture_export.ui',
				'graphics/html_export.ui',
                'graphics/suggestions.ui',
				'graphics/event_dialog.ui'])

moc_files = qt.compile_moc(headers : ['graphics/mainwindow.h', 'graphics/dialogs.h', 'graphics/graphics_items.h'])

# Icon
icon_file = 'icon.ico'
install_data(files('icon.ico'))

# Create executable.
executable('rodoc',
	source, moc_files, ui_files, resources,
	dependencies : [json_dep, qt_dep],
	install : true,
	install_dir : 'bin',
	resources : icon_file)
