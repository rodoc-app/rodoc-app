/// @file mainwindow.h This header file contains everything around the main window.
#ifndef mainwindow_h_
#define mainwindow_h_

#include <QtCore>
#include <QtWidgets>
#include <string>
#include <cstdlib>

#include "ui_mainwindow.h"

#include "dialogs.h"
#include "graphics_items.h"
#include "../core/family_tree.h"
#include "../core/strings.h"

QT_BEGIN_NAMESPACE
/// Namespace for classes having all its user interface items.
namespace Ui {
    /// User interface of the main window.
    class MainWindow;
}
QT_END_NAMESPACE

/// Class representing main window using Qt library.
class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	/// Default constructor.
	/// @param parent Qt parent.
	explicit MainWindow(QWidget *parent = nullptr);
	/// Default destructor, which deletes all dynamic attributes.
	~MainWindow();
	/// Export close family diagram.
	/// @param scale What is the scale for this picture.
	/// @param filename Where the picture should be saved, if it is empty prompt a dialog to choose file.
	/// @return If the export was successful or not.
	bool exportCloseFamilyDiagram(double scale, const std::string& filename = EMPTY_STRING);
	/// Export tree diagram.
	/// @param scale What is the scale for this picture.
	/// @param filename Where the picture should be saved, if it is empty prompt a dialog to choose file.
	/// @return If the export was successful or not.
	bool exportTreeDiagram(double scale, const std::string& filename = EMPTY_STRING);
	/// Set the number of generations to show.
	/// @param up Number of generations above (older).
	/// @param down Number of generations below (younger).
	/// @return Values that were set before (in the same order, first up then down).
    std::pair<int, int> setNumberOfGenerations(int up, int down);
    /// Open general file in an external app using QDesktopService if possible otherwise opening with a command.
    /// @param fileURL URL to the file.
    static void openExternalFile(const std::string& fileURL);
public slots:
    /// Show dialog for adding event.
    void addNewEvent();
	/// Show dialog for adding file.
	void addNewFile();
	/// Add new folder to the file tree.
	void addNewFileFolder();
	/// Show dialog for adding media.
	void addNewMedia();
	/// Add new folder to the media tree.
	void addNewMediaFolder();
	/// Show dialog for adding note.
	void addNewNote();
	/// Add new folder to the note tree.
	void addNewNoteFolder();
    /// Add new person to the family tree.
	void addNewPerson();
	/// Add new relation to the person by opening dialog window.
	void addNewRelation();
	/// Set to show all generations above.
	/// @param selected If all generations should be shown.
	void allAboveGenerations(bool selected);
    /// Set to show all generations below.
	/// @param selected If all generations should be shown.
	void allBelowGenerations(bool selected);
	/// Change the number of shown relations below.
	/// @param value How many generations to show.
	void changeGenerationsDown(int value);
	/// Change the number of shown relations above.
	/// @param value How many generations to show.
	void changeGenerationsUp(int value);
	/// If any value in persons info tab was changed, check its value, store it and refresh ui elements.
	void checkStoreRefreshPersonsInfo();
	/// Clear the project filter.
	void clearProjectFilter();
	/// Clear project paths.
	void clearProjectPaths();
	/// Hide new folder edit for files and cancel button.
	void disableFileFolderPrompt();
	/// Hide new folder edit for media and cancel button.
	void disableMediaFolderPrompt();
	/// Hide new folder edit for notes and cancel button.
	void disableNoteFolderPrompt();
	/// Edit event when clicking on it by opening a dialog window.
	/// @param item Which item was clicked.
	void editEvent(QTreeWidgetItem* item);
	/// Edit relation when clicking on it by opening a dialog window.
	/// @param item Which item was clicked.
	void editRelation(QTreeWidgetItem* item);
	/// Edit tag when clicking on it or creating new one.
	/// @param item Which item was selected, if any.
	void editTag(QTreeWidgetItem* item = nullptr);
	/// Show new folder edit for files and cancel button.
	void enableFileFolderPrompt();
	/// Show new folder edit for media and cancel button.
	void enableMediaFolderPrompt();
	/// Show new folder edit for notes and cancel button.
	void enableNoteFolderPrompt();
	/// Open dialog for exporting close family diagram.
	void exportCloseFamilyDiagramDialog();
	/// Export whole HTML project to an output directory by opening a dialog.
	void exportHtml();
	/// Export only main person html by opening a dialog.
	void exportMainHtml();
	/// Open dialog for exporting project.
	void exportProject();
	/// Open dialog for exporting templates.
	void exportTemplates();
	/// Open dialog for exporting family tree diagram.
	void exportTreeDiagramDialog();
	/// Filter only matching items in project.
	void filterProjectItems();
	/// Focus find in project view.
	void findPerson();
	/// Fit the tree view inside.
	void fitTreeInView();
	/// Choose file to import from other project.
	void importProject();
	/// Choose root directory of other project to import from.
	void importRootDir();
	/// Choose file to import templates.
	void importTemplates();
    /// Show the project item upon clicking it. It depends on the type of the item.
    /// For files open the file (same for notes and media) for person select it as a main.
    /// For events and relations open dialog with editing them.
    /// @param item Which item was clicked on.
    void interractWithProjectItem(QTreeWidgetItem* item);
    /// Open dialog for custom Birth Date.
	void openBirthDateDialog();
	/// Open dialog for custom Death Date.
	void openDeathDateDialog();
	/// Open event templates dialog.
    void openEventTemplatesDialog();
    /// Open existing root directory of a project.
    void openExistingProject();
    /// Open dialog for graphic settings.
    void openGraphicSettings();
    /// Open new empty project.
    void openNewFamilyTreeProject();
    /// Open relation templates dialog.
    void openRelationTemplatesDialog();
	/// Open file after double clicking it.
	/// @param item Which item was clicked on.
	void openSelectedFile(QTreeWidgetItem* item);
	/// Open media after double clicking it.
	/// @param item Which item was clicked on.
	void openSelectedMedia(QTreeWidgetItem* item);
	/// Open note after double clicking it.
	/// @param item Which item was clicked on.
	void openSelectedNote(QTreeWidgetItem* item);
	/// Promote current relation.
	void promoteRelation();
	/// Refresh all dates in persons info tab.
	void refreshDates();
	/// Refresh list with events.
	void refreshEventTab();
	/// Refresh all graphics.
	void refreshGraphics();
    /// Refresh the whole project that is shown.
    void refreshProjectView();
    /// Refresh relations lists.
    void refreshRelationTab();
    /// Refresh all files tabs.
    void refreshTabsWithFiles();
    /// Refresh shown tags.
	void refreshTagsList();
	/// Remove event from family tree.
	void removeEvent();
    /// Remove file folder.
	void removeFileFolder();
	/// Remove media folder.
	void removeMediaFolder();
	/// Remove note folder.
	void removeNoteFolder();
	/// Remove main person.
	void removePerson();
	/// Remove current relation.
	void removeRelation();
	/// Delete current tag selected on the person.
	void removeTag();
	/// Rename file or folder with files.
	void renameFile();
	/// Rename media or folder with media.
	void renameMedia();
	/// Rename note or folder with notes.
	void renameNote();
	/// Save the database.
	/// @return If the database was stored.
	bool saveDatabase();
	/// Save graphics settings from its dialog.
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
    void saveGraphicsSetting(QColor probandColor, QColor promotedColor, QColor standardColor, QColor highlightedColor, int sizeX, int sizeY, QFont font,
                             QColor lineColor, QColor textColor, int lineWidth, int rounding);
	/// De-select all other relation boxes other than older.
	void selectOlderGenerationOnly();
	/// De-select all other relation boxes other than same.
	void selectSameGenerationOnly();
	/// De-select all other relation boxes other than younger.
	void selectYoungerGenerationOnly();
	/// Change the visibility of all widgets related to death attributes.
	/// @param disabled If the widgets should be disabled or not.
	void setEnabledDeathAtributes(bool disabled);
	/// Hide or show the event tab.
	void setEventTabVisible();
	/// Hide or show the file tab.
	void setFileTabVisible();
	/// Hide or show the general tab.
	void setGeneralTabVisible();
	/// Set the main person by clicking on graphics item.
	/// @param personId Person id in the clicked box.
	void setMainPerson(size_t personId);
	/// Hide or show the media tab.
	void setMediaTabVisible();
	/// Hide or show the note tab.
	void setNoteTabVisible();
	/// Hide or show the relation tab.
	void setRelationTabVisible();
	/// Set the setting of the tab to east.
	void setTabEast();
	/// Set the setting of the tab to north.
	void setTabNorth();
	/// Set the setting of the tab to south.
	void setTabSouth();
	/// Set the setting of the tab to west.
	void setTabWest();
    /// Show generic help.
    void showHelp();
	/// Show prompt for renaming file.
	void showRenameFile();
	/// Show prompt for renaming media.
	void showRenameMedia();
	/// Show prompt for renaming note.
	void showRenameNote();
	/// When the splitter changed.
	void splitterChanged();
	/// Save tree and create new empty tree to show.
	void switchNewTreeProject();
	/// Zoom in on the family tree view.
	void zoomIn();
	/// Zoom out of the family tree view.
    void zoomOut();
protected:
    /// When closing the window check if the project is saved or not.
    /// @param event Called event.
    void closeEvent(QCloseEvent *event);
private:
    /// Open add file dialog for a person. This is as general as it gets.
	/// @param type Type of the file, that will be added.
	/// @param extensions Which extensions will be shown for creating empty file.
	/// @param item Which item is selected to put file.
	void addGeneralFile(FileType type, const std::vector<std::string>& extensions, FileTreeItem* item);
	/// Add folder to its type tree widget.
	/// @param type What widget it should use.
	void addGeneralFolder(FileType type);
	/// Radius of the boxes.
    int borderRadius_;
	/// Clear all UI elements.
	void clearUi();
    /// Connect all slots to their signals.
	void connectAllSlots();
	/// Edit event by its id.
	/// @param id Id of the event.
	void editEvent(size_t id);
	/// Edit relation by its id.
	/// @param id Id of the relation.
	void editRelation(size_t id);
	/// Show or hide new folder prompt.
	/// @param visible If the prompt should be visible or not.
	/// @param type For which type we are showing/hiding the prompt.
	void enableGeneralFolderPrompt(bool visible, FileType type);
	/// Show rename file prompt.
	/// @param type Which type is to be shown.
	void enableGeneralFolderRenamePrompt(FileType type);
	/// Export general picture of a diagram.
	/// @param filename What is the output file path.
	/// @param gscene Which scene to use.
	/// @param scale That is set for this picture.
	/// @return If the export was successful or not.
	bool exportGeneralDiagram(const QString& filename, QGraphicsScene* gscene, double scale = 1);
	/// Used font for view.
    QFont font_;
	/// Family tree.
	FamilyTree FT;
	/// Draw generalized cell, line and all inside person items in close family view. Like this:<br>
	/// ~~~{.txt}
	/// <-----sizeX--------->
	///
	/// topLeftX & topLeftY
	/// v
    /// +-------cell--------+   ^
    /// |gap|box|gap|box|gap| sizeY
    /// +-------------------+   v
    ///           | line to middle box
    /// ~~~
    /// @param container Container of persons which will be drawn.
    /// @param sizeX Size of the cell in the x axis.
    /// @param sizeY Size of the cell in the y axis.
    /// @param topLeftX Top left point of the cell in the x axis.
    /// @param topLeftY Top left point of the cell in the y axis.
    /// @param lineX1 Starting point of the line in x axis.
    /// @param lineY1 Starting point of the line in y axis.
    /// @param lineX2 Ending point of the line in x axis.
    /// @param lineY2 Ending point of the line in y axis.
    /// @param partner Index of partner to use special color.
    /// @param mother Index of mother to use special color.
    /// @param father Index of father to use special color.
    /// @param gap Gap which is between boxes in cell.
    /// @param movex Whether to move boxes on the x line.
    /// @param movey Whether to move boxes on the y line.
	void drawCloseContainer(const std::vector<Relation*>& container,
                                    qreal sizeX, qreal sizeY, qreal topLeftX, qreal topLeftY,
                                    qreal lineX1, qreal lineY1, qreal lineX2, qreal lineY2,
                                    size_t partner, size_t mother, size_t father, int gap,
                                    bool movex, bool movey);
	/// Draw close family to the graphics.<br>
	/// ~~~{.txt}
	///   +------cell---------+
    ///   |gap|box|gap|box|gap|
    ///   +-------------------+
    ///+---+<-cell  |
    ///|gap|    +-------+ <-- box
    ///|box|----|proband|
    ///|gap|    +-------+
    ///+---+        |
    ///   +------cell---------+
    ///   |gap|box|gap|box|gap|
    ///   +-------------------+
    /// ~~~
	void drawCloseFamily();
	/// Draw family tree to the corresponding person.<br>
	/// ~~~{.txt}
	///  ....           ....
	///   \/             \/
	///+--box--+     +--box--+
	///|father |     |mother |
	///+-------+     +-------+
	///        \     /
	///       +--box--+
	///       |proband|
	///       +-------+
    ///        |     |
	///+--box--+     +--box--+
	///|child  |     |child  |
	///+-------+     +-------+
    ///   /|\            |
    ///          ...
    /// ~~~
	void drawFamilyTree();
	/// Draw either top or bottom generation in tree.
    /// @param p Current person.
    /// @param width Width of the place where will be the relations.
    /// @param midx What is the middle in the x axis of the place.
    /// @param midy What is the middle in the y axis of the place.
    /// @param horizontalGap Min gap between boxes in the x axis.
    /// @param verticalGap Gap between generations.
	/// @param barrier How more generations to show.
	/// @param container Vector of relations to draw.
	/// @param up If we are drawing upwards or downwards.
	/// @param numberFunc Function for computing the widths and depths of the trees.
	/// @param drawFunc Function to recursively call for drawing.
    void drawGeneralGenerations(Person* p, qreal width, qreal midx, qreal midy, int horizontalGap, int verticalGap, size_t barrier,
                                    const std::vector<size_t>& container, bool up,
                                    std::function<std::pair<size_t, size_t>(Person*, size_t, size_t)> numberFunc,
                                    std::function<void(Person*, qreal, qreal, qreal, int, int, size_t)> drawFunc);
	/// Draw one family tree box and recursively make other.<br>
	/// ~~~{.txt}
	/// +----------------------+
	/// |<-xfrom  ....         | Box will be in the middle.
	/// |<-yfrom  ....         | Then it will call recursively on the top half the space.
	/// +----------------------+
	/// <--------space--------->
	/// ~~~
	/// @param p Which person we are drawing.
	/// @param width What is the x size where should the box take place.
	/// @param midx What is the middle in the x axis of the place.
    /// @param midy What is the middle in the y axis of the place.
	/// @param horizontalGap Min gap between boxes in the x axis.
    /// @param verticalGap Gap between generations.
	/// @param barrier How more generations to show.
	void drawOlderGenerations(Person* p, qreal width, qreal midx, qreal midy, int horizontalGap, int verticalGap, size_t barrier);
	/// Draw partner (to the left of the main person) to the family tree if there is one.
	/// @param canvasMidX Centre-point of the canvas.
	/// @param canvasMidY Centre-point of the canvas.
	/// @param horizontalGap Gap in between persons boxes.
	void drawPartner(qreal canvasMidX, qreal canvasMidY, int horizontalGap);
	/// Draw all younger relations to the tree diagram.<br>
	/// ~~~{.txt}
    /// +---+     +---+     +---+
    /// |box|     |box|     |box|
    /// +---+     +---+     +---+
    ///   .       /   \       .
    /// .....  +---+  +---+ .....
    /// .   .  |box|  |box| .   .
    /// .....  +---+  +---+ .....
    /// ~~~
    /// @param p Current person which is above.
    /// @param width Width of the place where will be the children drawn.
    /// @param midx What is the middle in the x axis of the place for children.
    /// @param midy What is the middle in the y axis of the place for children.
    /// @param horizontalGap Min gap between boxes in the x axis.
    /// @param verticalGap Gap between generations.
    /// @param barrier How more generations to show.
    void drawYoungerGenerations(Person* p, qreal width, qreal midx, qreal midy, int horizontalGap, int verticalGap, size_t barrier);
    /// Compute recursively the maximum number of younger or older generations for the given person.
    /// @param p Given person.
    /// @param soFar What is the depth so far.
    /// @param barrier How many more generations to show.
    /// @param container Vector of relations in given direction.
    /// @param up If the direction is up or not.
    /// @param func Which function to recursively call.
    /// @return First is what is the width and second is the depth of the tree.
	std::pair<size_t, size_t> generalGenerationNumber(Person* p, size_t soFar, size_t barrier,
                                                   const std::vector<size_t>& container, bool up,
                                                   std::function<std::pair<size_t, size_t>(Person*, size_t, size_t)> func);
    /// How many generations to show in tree view down.
    int genSizeDown_;
    /// How many generations to show in tree view up.
    int genSizeUp_;
	/// Color for highlighted boxes.
	QColor highlightedColor_;
	/// Pen used for drawing lines.
    QPen linePen_;
	/// Load the setting from configuration.
	void loadSettings();
	/// Get the number of upper generations for a person.
	/// @param p Which person we are looking at.
	/// @param soFar What is the number so far to the person p.
	/// @param barrier How more generations to show.
	/// @return First is what is the maximum to the width and second is how high the tree is.
	std::pair<size_t, size_t> olderGenerationNumber(Person* p, int soFar, size_t barrier);
	/// Open custom date dialog for a date.
	/// @param date Pointer to the date, that is being edited.
	void openCustomDateDialog(WrappedDate* date);
	/// Initialize all sub-folder for item in project.
	/// @param parent Parent item holding these sub items.
	/// @param person To which person it is bounded to.
	void initializeProjectSubView(ProjectItem* parent, Person* person);
	/// Initialize project view when loading new family tree.
	void initializeProjectView();
	/// Open file after double clicking it.
	/// @param item Which item was clicked.
	/// @param type What is the type of the file.
	void openGeneralFile(QTreeWidgetItem* item, FileType type);
	/// Open general project by opening existing one from the list or new one.
	/// @param dirPath Path to root directory of the project.
	void openGeneralProject(const std::string& dirPath);
	/// Color of proband.
	QColor probandColor_;
	/// Refresh basic information about main person.
    void refreshPersonsInfoTab();
	/// Refresh project view for events.
    /// @param item Group item for all events.
    /// @param person For which person we are showing events.
    void refreshProjectEventsView(ProjectItem* item, Person* person);
    /// Refresh project view with general files.
    /// @param containerIcon Icon of the container having all files.
    /// @param containerName Name of the container having all files.
    /// @param fileIcon Icon for the file itself.
    /// @param type Type of the file.
    /// @param item Which item is the container.
    /// @param person To which person these containers are bounded.
    void refreshProjectGeneralFilesView(const QIcon& containerIcon, const QString& containerName, const QIcon& fileIcon, FileType type, ProjectItem* item, Person* person);
    /// Refresh project view for files.
    /// @param item Group item for all files.
    /// @param person For which person we are showing files.
    void refreshProjectFilesView(ProjectItem* item, Person* person);
    /// Refresh project view for media.
    /// @param item Group item for all media.
    /// @param person For which person we are showing media.
    void refreshProjectMediaView(ProjectItem* item, Person* person);
    /// Refresh project view for notes.
    /// @param item Group item for all notes.
    /// @param person For which person we are showing notes.
    void refreshProjectNoteView(ProjectItem* item, Person* person);
    /// Refresh project view for relations.
    /// @param item Group item for all relations.
    /// @param person For which person we are showing relations.
    void refreshProjectRelationsView(ProjectItem* item, Person* person);
    /// Refresh the whole UI (tabs, project and graphics).
    void refreshUi();
	/// Remove folder from the tree widget.
	/// @param type Which type is used. To determine which tree widget is used.
	void removeGeneralFolder(FileType type);
	/// Rename general file tree item.
	/// @param type Which type is used. To determine which tree widget is used.
	void renameGeneralFileItem(FileType type);
    /// Scene shown on the graphical view.
	QGraphicsScene* scene;
    /// Save all basic information about main person.
    void savePersonsInfo();
    /// Set all additional static widgets of the user interface.
    void setUiElements();
    /// Show files from container to its widget.
	/// @param widget QTreeWidget to show all the files.
	/// @param root Root folder of all files.
	/// @param type Which type of files are going to be displayed.
	void showGeneralFiles(QTreeWidget* widget, VirtualDrive* root, FileType type);
	/// X size of the person box.
	int sizeX_;
	/// Y size of the person box.
	int sizeY_;
	/// Color for promoted father, mother and partner.
    QColor promotedColor_;
    /// Color for other relations.
    QColor standardColor_;
    /// Pen used for texts.
    QPen textPen_;
	/// Tree view scene;
    QGraphicsScene* treeScene;
    /// Tree view graphics view.
    GraphicsView* treeView;
	/// Qt UI framework.
	Ui::MainWindow* ui;
	/// Compute the maximum number of younger generations of the person.
	/// @param p Given person.
	/// @param soFar What is the depth so far.
	/// @param barrier How more generations to show.
	/// @return First is what is the maximum to the width and second is how deep the tree is.
	std::pair<size_t, size_t> youngerGenerationNumber(Person* p, size_t soFar, size_t barrier);
};

#endif
