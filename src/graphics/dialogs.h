/// @file dialogs.h This file contains all window dialogs that will be shown. These classes are relatively small but there are many of them.
#ifndef dialogs_h_
#define dialogs_h_

// Header files created from .ui files.
#include "ui_date_dialog.h"
#include "ui_tag_chooser.h"
#include "ui_add_file.h"
#include "ui_relation_templates.h"
#include "ui_event_templates.h"
#include "ui_event_dialog.h"
#include "ui_relation_dialog.h"
#include "ui_help_dialog.h"
#include "ui_choose_person.h"
#include "ui_visual_settings.h"
#include "ui_export_templates.h"
#include "ui_export_project.h"
#include "ui_html_export.h"
#include "ui_picture_export.h"
#include "ui_suggestions.h"

#include "graphics_items.h"
#include <QtCore>
#include <QtWidgets>
#include <string>
#include <typeinfo>
#include <iostream>
#include "../core/family_tree.h"
#include "../core/strings.h"
#include "mainwindow.h"

QT_BEGIN_NAMESPACE
/// Namespace for classes having all its user interface items.
namespace Ui {
    /// User interface of date dialog.
    class DateDialog;
    /// User interface of tag chooser.
    class TagChooserDialog;
    /// User interface of adding file dialog.
    class AddFileDialog;
    /// User interface of relation template dialog.
    class RelationTemplateDialog;
    /// User interface of event templates dialog.
    class EventTamplatesDialog;
    /// User interface of single event edit.
    class EventDialog;
    /// User interface of single relation edit.
    class RelationDialog;
    /// User interface of dialog with text edit.
    class HelpDialog;
    /// User interface of dialog for choosing person.
    class ChoosePersonDialog;
    /// User interface of dialog for setting the graphic parameters.
    class VisualSettings;
    /// User interface of dialog for exporting templates.
    class ExportTemplatesDialog;
    /// User interface of a dialog for exporting project.
    class ExportProjectDialog;
    /// User interface of a dialog for exporting Pictures.
    class PictureExport;
    /// User interface of a dialog for exporting HTML pages.
    class HtmlExport;
    /// User interface of a dialog for relation suggestions.
    class SuggestionsDialog;
}
QT_END_NAMESPACE

/// Dialog for editing custom dates.
class DateDialog : public QDialog {
	Q_OBJECT
    public:
        /// Default constructor.
        /// @param FT Pointer to the family tree.
        /// @param date Pointer to the used date.
        /// @param parent The Qt Widget parent.
        explicit DateDialog(FamilyTree* FT, WrappedDate* date = nullptr, QWidget* parent = nullptr);
        /// Default destructor.
        ~DateDialog();
    public slots:
        /// Slot for saving the date if OK button was pressed.
        void saveDate();
        /// If the unknown check box was changed.
        /// @param isUnknown If the check box was checked.
        void unknownDate(bool isUnknown);
    private:
        /// Pointer to the used date.
        WrappedDate* date_;
        /// Pointer to the family tree.
        FamilyTree* FT_;
        /// User interface of the Qt framework.
        Ui::DateDialog *ui;
};

/// Dialog for editing or creating tags for a person.
class TagChooserDialog : public QDialog{
	Q_OBJECT
    public:
        /// Default constructor.
        /// @param FT Pointer to the family tree.
        /// @param tag What to show as a tag.
        /// @param value What to show as a value of the tag.
        /// @param parent The Qt Widget parent.
        TagChooserDialog(FamilyTree* FT, const QString& tag, const QString& value, QWidget* parent = nullptr);
        /// Default destructor.
        ~TagChooserDialog();
    public slots:
        /// Slot for showing/hiding custom tag edit.
        /// @param index Which index was from the combo box chosen.
        void changeVisibleWidgets(int index);
        /// Save the tag.
        void saveTag();
    private:
        /// Pointer to the family tree.
        FamilyTree* FT_;
        /// If the tag is being edited or new one is being created.
        bool newOne_;
        /// User interface of the Qt framework.
        Ui::TagChooserDialog *ui;
};

/// Dialog for adding file.
class AddFileDialog : public QDialog{
	Q_OBJECT
    public:
        /// Default constructor.
        /// @param type If the file is general file, media or note.
        /// @param extensions Vector of all extensions to show.
        /// @param parentDrive Parent drive where the file will be stored.
        /// @param FT Given family tree the current app is using.
        /// @param parent The Qt Widget parent.
        AddFileDialog(FileType type, const std::vector<std::string>& extensions, VirtualDrive* parentDrive, FamilyTree* FT, QWidget* parent);
        /// Default destructor.
        ~AddFileDialog();
    public slots:
        /// Open file chooser for copying file.
        void copyFile();
        /// Create or choose file based on selected tab.
        void createOrChooseFile();
        /// Show file upon clicking it.
        /// @param item Which item was clicked.
        void openFile(QTreeWidgetItem* item);
    private:
        /// Used family tree.
        FamilyTree* FT_;
        /// Get the id from the text of the given id.
        /// @param item Which item has the id.
        /// @return Id from its text.
        size_t getId(QTreeWidgetItem* item);
        /// Parent drive.
        VirtualDrive* drive_;
        /// Type of the file.
        FileType type_;
        /// User interface of the Qt framework.
        Ui::AddFileDialog* ui;
};

/// Dialog showing all templates and possibly editing them or creating new ones.
class RelationTemplateDialog : public QDialog{
    Q_OBJECT
    public:
        /// Default constructor.
        /// @param FT Pointer to the family tree.
        /// @param parent The Qt Widget parent.
        explicit RelationTemplateDialog(FamilyTree* FT, QWidget* parent = nullptr);
        /// Default destructor.
        ~RelationTemplateDialog();
    public slots:
        /// Add default templates.
        void addDefaults();
        /// Clear boxes and set them for new template.
        void addNewTemplate();
        /// Remove current template from the templates.
        void removeTemplate();
        /// Either save edits to the template or add new item.
        void saveTemplate();
        /// Show template details to its text boxes.
        /// @param item Which item was clicked.
        void showTemplate(QTreeWidgetItem* item);
    private:
        /// When we are adding new template.
        bool addingNew_;
        /// Clear all text boxes.
        void clear();
        /// Pointer to the family tree.
        FamilyTree* FT_;
        /// Refresh list of all relation templates.
        void refreshListWidget();
        /// Set enabled for all widgets.
        /// @param enabled If the widgets should be enabled or not.
        void setEnabledWidgets(bool enabled);
        /// Store the given template from ui boxes.
        /// @param rel Pointer to which relation to overwrite.
        void saveTemplate(RelationTemplate* rel);
        /// User interface of the Qt framework.
        Ui::RelationTemplateDialog* ui;
};

/// Dialog showing all event templates and possibly editing them or creating new ones.
class EventTamplatesDialog : public QDialog{
    Q_OBJECT
    public:
        /// Default constructor.
        /// @param FT Pointer to the family tree.
        /// @param parent The Qt Widget parent.
        explicit EventTamplatesDialog(FamilyTree* FT, QWidget* parent = nullptr);
        /// Default destructor.
        ~EventTamplatesDialog();
    public slots:
        /// Add default templates.
        void addDefaults();
        /// Add new role to the list.
        void addNewRole();
        /// Show empty boxes to add new template.
        void addNewTemplate();
        /// Remove role from the list.
        void removeRole();
        /// Remove template from the list.
        void removeTemplate();
        /// Save template or create a new one.
        void saveTemplate();
        /// Show template details.
        /// @param item Which item is clicked.
        void showTemplate(QTreeWidgetItem* item);
    private:
        /// When we are adding new template.
        bool addingNew_;
        /// Clear all boxes.
        void clear();
        /// Pointer to the family tree.
        FamilyTree* FT_;
        /// Refresh list of all relation templates.
        void refreshListWidget();
        /// Overwrite data of the event template by the input from ui.
        /// @param event Pointer to which template to overwrite.
        void saveTemplate(EventTemplate* event);
        /// Set enabled for all widgets.
        /// @param enabled If the widgets should be enabled or not.
        void setEnabledWidgets(bool enabled);
        /// User interface of the Qt framework.
        Ui::EventTamplatesDialog* ui;
};

/// Dialog for choosing person with find window. Parent have to have slot `savePerson()`.
class ChoosePersonDialog : public QDialog{
    Q_OBJECT
    public:
        /// Default constructor.
        /// @param FT Pointer to the family tree.
        /// @param person Pointer to the person to be replaced or nullptr to choose new one.
        /// @param parent The Qt Widget parent.
        ChoosePersonDialog(FamilyTree* FT, Person* person, QWidget* parent = nullptr);
        /// Default destructor.
        ~ChoosePersonDialog();
    signals:
        /// When save can be proceeded.
        void savePerson(Person* person);
    public slots:
        /// Clear filter and show all persons.
        void clearFilter();
        /// Filter persons by the text edit.
        void filterPersons();
        /// Save currently selected person.
        void saveSelectedPerson();
    private:
        /// Pointer to the family tree.
        FamilyTree* FT_;
        /// Pointer for
        Person* person_;
        /// User interface of the Qt framework.
        Ui::ChoosePersonDialog* ui;
};

/// Dialog showing single relation for editing it.
class RelationDialog : public QDialog{
    Q_OBJECT
    public:
        /// Default constructor.
        /// @param FT Pointer to the family tree.
        /// @param parent The Qt Widget parent.
        /// @param rel Which relation is to be added or edited.
        explicit RelationDialog(FamilyTree* FT, QWidget* parent = nullptr, Relation* rel = nullptr);
        /// Default destructor.
        ~RelationDialog();
    public slots:
        /// Slot for opening dialog to save first person.
        void chooseFirstPerson();
        /// Slot for opening dialog to save second person.
        void chooseSecondPerson();
        /// Save person upon choosing it from another dialog.
        void savePerson(Person* person);
        /// Save relation to the family tree or add new one and save it.
        void saveRelation();
        /// Show details about current template.
        void showTemplateDetails();
        /// Switch the persons.
        void switchPersons();
    private:
        /// Pointer to the family tree.
        FamilyTree* FT_;
        /// First person of the relation.
        Person* person1_;
        /// Second person of the relation.
        Person* person2_;
        /// Pointer to the edited relation, if adding new one it is a null pointer.
        Relation* rel_;
        /// Whether the first person is to be saved or the second.
        bool saveFirstPerson_;
        /// User interface of the Qt framework.
        Ui::RelationDialog* ui;
};

/// Dialog showing single event for editing it.
class EventDialog : public QDialog{
    Q_OBJECT
    public:
        /// Default constructor.
        /// @param FT Pointer to the family tree.
        /// @param parent The Qt Widget parent.
        /// @param event Which event is being edited if any.
        explicit EventDialog(FamilyTree* FT, QWidget* parent = nullptr, Event* event = nullptr);
        /// Default destructor.
        ~EventDialog();
    public slots:
        /// Add empty role to the roles.
        void addEmptyRole();
        /// Open dialog for choosing person.
        void choosePerson();
        /// Open dialog for custom date.
        void openCustomDate();
        /// Refresh date from dialog.
        void refreshDates();
        /// Refresh persons comboBox to the current role and its value.
        /// @param item Which item was clicked on.
        void refreshPersons(QTreeWidgetItem* item);
        /// Remove empty role from the list.
        void removeEmptyRole();
        /// Remove person from its role.
        void removePerson();
        /// Save the data to the event.
        void saveEvent();
        /// Save person from dialog.
        /// @param person Which person was selected.
        void savePerson(Person* person);
        /// Show the template details.
        void showTemplate();
    private:
        /// Wrapped date of this event.
        WrappedDate date_;
        /// Pointer to the event that is being edited, may be a null pointer.
        Event* event_;
        /// Pointer to the family tree.
        FamilyTree* FT_;
        /// Pointer to the main person used if no other persons are in event.
        Person* main_;
        /// Show event details.
        void showEventDetails();
        /// User interface of the Qt framework.
        Ui::EventDialog*  ui;

};

/// Dialog with text edit to show help or other texts.
class HelpDialog : public QDialog{
    Q_OBJECT
    public:
        /// Default constructor.
        /// @param FT Pointer to the family tree.
        /// @param parent The Qt Widget parent.
        explicit HelpDialog(FamilyTree* FT, QWidget* parent = nullptr);
        /// Default destructor.
        ~HelpDialog();
    public slots:
        /// If help dialog should be opened at start-up.
        /// @param help Forwarding of check box in UI. True for not showing.
        void openHelp(bool help);
    private:
        /// Pointer to the family tree.
        FamilyTree* FT_;
        /// User interface of the Qt framework.
        Ui::HelpDialog* ui;
};

/// Dialog for editing and showing graphic settings.
class VisualSettings : public QDialog{
    Q_OBJECT
    public:
        /// Default constructor.
        /// @param probandColor Color used for proband.
        /// @param promotedColor Color for promoted Relations.
        /// @param standardColor Standard color.
        /// @param highlighted Color for highlighting the box.
        /// @param sizeX Size of the box in x axis.
        /// @param sizeY Size of the box in y axis.
        /// @param font Used font for the text in the box.
        /// @param pen Which pen is used for lines.
        /// @param textPen Which pen is used for text.
        /// @param borderRadius What is the border radius of the box.
        /// @param parent The Qt Widget parent.
        VisualSettings(QColor probandColor, QColor promotedColor, QColor standardColor, QColor highlighted, int sizeX, int sizeY, QFont font,
                        QPen* pen, QPen* textPen, int borderRadius, QWidget* parent = nullptr);
        /// Default destructor.
        ~VisualSettings();
    signals:
        /// Signal to emit when OK button is pressed to save this configuration.
        /// @param probandColor Color used for proband.
        /// @param promotedColor Color for promoted Relations.
        /// @param standardColor Standard color.
        /// @param highlightedColor Highlighted color.
        /// @param sizeX Size of the box in x axis.
        /// @param sizeY Size of the box in y axis.
        /// @param font Used font for the text in the box.
        /// @param lineColor Color for lines.
        /// @param textColor Color for texts.
        /// @param lineWidth Width of the lines.
        /// @param rounding Rounding of the boxes.
        void saveSettings(QColor probandColor, QColor promotedColor, QColor standardColor, QColor highlightedColor, int sizeX, int sizeY, QFont font,
                          QColor lineColor, QColor textColor, int lineWidth, int rounding);
    public slots:
        /// Show dialog for color change upon clicking on highlighted color change.
        void highlightedColor();
        /// Change the color of line.
        void lineColor();
        /// Upon clicking the refresh button load the values and show the graphics.
        void refresh();
        /// Show dialog for color change upon clicking on proband color change.
        void probandColor();
        /// Show dialog for color change upon clicking on promoted color change.
        void promotedColor();
        /// Emit signal for saving the configuration and exit the dialog.
        void saveAndExit();
        /// Show dialog for color change upon clicking on standard color change.
        void standardColor();
        /// Change the color of a text.
        void textColor();
    private:
        /// Border radius of the box.
        int borderRadius_;
        /// Used font.
        QFont font_;
        /// Color for highlighting the box.
        QColor highlightedColor_;
        /// Which pen is used for lines.
        QPen pen_;
        /// Color for proband.
        QColor probandColor_;
        /// Color for promoted relations.
        QColor promotedColor_;
        /// Used graphics scene.
        QGraphicsScene* scene;
        /// Size of the box in x axis.
        int sizeX_;
        /// Size of the box in y axis.
        int sizeY_;
        /// Color for standard relations.
        QColor standardColor_;
        /// Pen used for texts.
        QPen textPen_;
        /// Used Qt user interface.
        Ui::VisualSettings* ui;
};

/// Dialog for exporting some of the event and relation templates.
class ExportTemplatesDialog : public QDialog{
    Q_OBJECT
    public:
        /// Default constructor.
        /// @param FT Pointer to the family tree.
        /// @param parent The Qt Widget parent.
        explicit ExportTemplatesDialog(FamilyTree* FT, QWidget* parent = nullptr);
        /// Default destructor.
        ~ExportTemplatesDialog();
    public slots:
        /// When OK button was pressed load all selected templates and export them.
        void exportTemplates();
    private:
        /// Pointer to the family tree.
        FamilyTree* FT_;
        /// Used Qt user interface.
        Ui::ExportTemplatesDialog* ui;
};

/// Dialog for exporting project.
class ExportProjectDialog : public QDialog{
    Q_OBJECT
    public:
        /// Default constructor.
        /// @param FT Pointer to the used Family tree.
        /// @param parent The Qt Widget parent.
        explicit ExportProjectDialog(FamilyTree* FT, QWidget* parent = nullptr);
        /// Default destructor.
        ~ExportProjectDialog();
    public slots:
        /// Let the person check box be always checked.
        void checkPerson();
        /// Export project upon clicking OK.
        void exportProject();
    private:
        /// Pointer to the family tree.
        FamilyTree* FT_;
        /// Used Qt user interface.
        Ui::ExportProjectDialog* ui;
};

/// Dialog for exporting diagram. To choose zoom of the picture.
class PictureExport : public QDialog{
    Q_OBJECT
    public:
        /// Default constructor.
        /// @param closeDiagram If the diagram should be close family diagram. Otherwise it is family tree.
        /// @param parent The Qt Widget parent.
        explicit PictureExport(bool closeDiagram, QWidget* parent = nullptr);
        /// Default destructor.
        ~PictureExport();
    public slots:
        /// When OK is pressed export the diagram.
        void exportDiagram();
    private:
        /// Which diagram is being exported.
        bool closeDiagram_;
        /// Used Qt user interface.
        Ui::PictureExport* ui;

};

/// Dialog for exporting HTML for one person and all persons.
class HtmlExport : public QDialog{
    Q_OBJECT
    public:
        /// Default constructor.
        /// @param FT Pointer to the used Family tree.
        /// @param all If the HTML export should include all persons or just the main person.
        /// @param up How many generations upwards are being exported as of now.
        /// @param down How many generations downwards are being exported as of now.
        /// @param parent The Qt Widget parent.
        HtmlExport(FamilyTree* FT, bool all, int up, int down, QWidget* parent = nullptr);
        /// Default destructor.
        ~HtmlExport();
    public slots:
        /// If all generations upward option was chosen or not.
        /// @param all True if it was checked.
        void allAboveGenerations(bool all);
        /// If all generations downward option was chosen or not.
        /// @param all True if it was checked.
        void allBelowGenerations(bool all);
        /// Export all persons into html.
        void exportAllHtml();
        /// Export only main person to the html.
        void exportMainHtml();
    private:
        /// Pointer to the family tree.
        FamilyTree* FT_;
        /// Used Qt user interface.
        Ui::HtmlExport* ui;
};

/// Dialog for relation suggestions.
class SuggestionsDialog : public QDialog{
    Q_OBJECT
    public:
        /// Default constructor.
        /// @param FT Pointer to the family tree.
        /// @param suggestions All the suggestions for relations.
        /// @param parent The Qt Widget parent.
        SuggestionsDialog(FamilyTree* FT, std::vector<RelationSuggestion> suggestions, QWidget* parent = nullptr);
        /// Default destructor.
        ~SuggestionsDialog();
    public slots:
        /// Remove current suggestion.
        void removeSuggestion();
        /// Save all suggestions.
        void save();
        /// When a suggestion is clicked.
        /// @param item Which suggestion was clicked.
        void selectedSuggestion(QTreeWidgetItem* item);
        /// Swap persons roles.
        void swapPersons();
        /// Switch the template description.
        void switchedTemplate();
    private:
        /// Vector of all templates for fatherhood relations.
        std::vector<RelationTemplate*> fatherhoods;
        /// Pointer to the family tree.
        FamilyTree* FT_;
        /// Vector of all templates for motherhood relations.
        std::vector<RelationTemplate*> motherhoods;
        /// Vector of all templates for partnership relations.
        std::vector<RelationTemplate*> partnerships;
        /// Vector of all templates for siblings relations.
        std::vector<RelationTemplate*> siblings;
        /// Used Qt user interface.
        Ui::SuggestionsDialog* ui;
};

#endif
