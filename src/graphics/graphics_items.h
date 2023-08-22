/// @file graphics_items.h This files contains all graphics items, that are inherited from Qt framework.
#ifndef graphics_items_h_
#define graphics_items_h_

#include <QtCore>
#include <QtWidgets>
#include <string>
#include <typeinfo>
#include <iostream>
#include "../core/family_tree.h"
#include "../core/strings.h"

/// To clarify that the value contains a forbidden character.
/// @param value Which value is checked.
/// @return True if it contains any of forbidden characters.
bool containsForb(const QString& value);

/// TreeWidget Item for general file.
class FileTreeItem : public QTreeWidgetItem{
	public:
		/// Construct this drive and all sub-drives.
		/// @param drive Which drive is presented in this item.
		/// @param FT Pointer to the family tree.
		/// @param icon Which icon to show beside the text.
		/// @param type Type of the file to show.
		FileTreeItem(VirtualDrive* drive, FamilyTree* FT, const QIcon& icon, FileType type);
		/// Constructor for a new file.
		/// @param id Id of the file.
		/// @param FT Pointer to the family tree.
		/// @param icon Which icon is shown besides files.
		/// @param type What type of files are shown.
		FileTreeItem(size_t id, FamilyTree* FT, const QIcon& icon, FileType type);
		/// Add file to this folder.
		/// @param id Id of the file.
		/// @param icon Which icon to show besides the text.
		void addFile(size_t id, const QIcon& icon);
		/// Add sub drive to this drive.
		/// @param folder The name of the sub-folder.
		void addSubDrive(const QString& folder);
		/// Get the pointer to the drive.
		/// @return Pointer to the drive.
		VirtualDrive* getDrive();
		/// Get the id of the file.
		/// @return File id.
		size_t getId();
		/// If te file is directory or not.
		/// @return True if it is directory, false otherwise.
		bool isDirectory();
		/// Remove recursively all directories and files.
		/// @return True if it is the root directory, false otherwise.
		bool removeFolder();
		/// When this item is selected for renaming.
		/// @param name Name for this folder.
		/// @return True if this item was file, false otherwise.
        bool rename(const QString& name);
	private:
	    /// Drive presented in this folder.
	    VirtualDrive* drive_;
	    /// Id of the shown file.
		size_t fileId_;
		/// Pointer to the family tree.
		FamilyTree* FT_;
		/// If the file is directory or not.
		bool isDir_;
		/// Remove file in this drive.
		/// @param id Id of the file.
        void removeFile(size_t id);
		/// Type of the file.
		FileType type_;
};

/// What type of a item is to be shown.
enum ProjectItemType {PROJECT_PERSON, PROJECT_GROUP, PROJECT_FILE, PROJECT_MEDIA, PROJECT_NOTE, PROJECT_EVENTS, PROJECT_RELATIONS};

/// This is a tree widget item to be shown on a project view.
class ProjectItem : public QTreeWidgetItem{
    public:
        /// Default constructor.
        /// @param type What is the type of this item.
        /// @param id Given id of the item.
        explicit ProjectItem(ProjectItemType type, size_t id = 0);
        /// Get the id of the object.
        /// @return Id of the object.
        size_t getId() const;
        /// Get the type of the item.
        /// @return Type of the project.
        ProjectItemType type() const;
    private:
        /// Id of the stored object.
        size_t id_;
        /// Type of the stored object.
        ProjectItemType type_;
};

/// Tree widget item for persons in events.
class EventRoleItem : public QTreeWidgetItem{
    public:
        /// Default constructor.
        /// @param role String representing the role.
        /// @param id Id of the person if any.
        explicit EventRoleItem(const std::string& role, size_t id = 0);
        /// Get the id of the person.
        /// @return Id of the person.
        size_t getId();
        /// Set person to the role.
        /// @param name Name of the person.
        /// @param id Id of the person.
        void setPerson(const QString& name, size_t id = 0);
    private:
        /// Id of the person.
        size_t id_;
};

/// Box with persons information.
class PersonsGraphicsItem : public QObject, public QGraphicsItem{
    Q_OBJECT
    public:
        /// Default constructor.
        /// @param lines Each separate line of text to show in the box.
        /// @param sizex Size of the cell in the x axis.
        /// @param sizey Size of the cell in the y axis.
        /// @param p Given person represented in this box.
        /// @param x Top left point x position.
        /// @param y Top left point y position.
        /// @param color Which color will be used to fill the box.
        /// @param highlighted Which color will be used when the box is highlighted.
        /// @param font Which font will be used to type the words.
        /// @param parent Which widget will take care of clicking the box.
        /// @param pen Which pen is used for lines.
        /// @param textPen Which pen is used for text.
        /// @param borderRadius What is the radius of the border.
        PersonsGraphicsItem(const QStringList& lines, int sizex, int sizey, Person* p, qreal x, qreal y,
                            QColor color, QColor highlighted, QFont font, QWidget* parent, QPen* pen, QPen* textPen, int borderRadius);
        /// Default destructor.
        ~PersonsGraphicsItem();
        /// Get the bounding rectangle of the object.
        /// @return Rectangle that is bounding the item.
        QRectF boundingRect() const override;
        /// Paint the object to its painter.
        /// @param painter Which painter to use to paint the object.
        /// @param option Option for the Qt framework (not used).
        /// @param widget Widget for the Qt framework (not used).
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
        /// Get the shape of the item.
        /// @return Get the shape to paint.
        QPainterPath shape() const override;
    signals:
        /// Signal when the item is pressed.
        void itemClicked(size_t personId);
    protected:
        /// When the box is hovered by a mouse highlight the box.
        /// @param event Event when the item was hovered.
        void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
        /// When the box is not hovered anymore by a mouse stop highlighting.
        /// @param event Event when the item is not hovered anymore.
        void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
        /// Press event after clicking the object.
        /// @param event Event when the item was clicked.
        void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    private:
        /// Border radius of the box.
        int borderRadius_;
        /// Which text should be bold.
        int bold_text_;
        /// Which color will be used to fill the box.
        QColor color_;
        /// Which font is used.
        QFont font_;
        /// Which color will be used when highlighted.
        QColor highlighted_;
        /// If the box is hovered.
        bool hovered_;
        /// Pointer to the used pen.
        QPen* pen_;
        /// Given person represented in this box.
        Person* person_;
        /// Size of the cell in the x axis.
        int sizex_;
        /// Size of the cell in the y axis.
        int sizey_;
        /// All text lines to show in the text box.
        QStringList textLines_;
        /// Pointer to the text pen.
        QPen* textPen_;
        /// Top left point x position.
        qreal x_;
        /// Top left point y position.
        qreal y_;
};

/// Cell around all relations.
class CellGraphicsItem : public QGraphicsItem{
    public:
        /// Default constructor.
        /// @param sizex Size of the cell in the x axis.
        /// @param sizey Size of the cell in the y axis.
        /// @param x Top left point x position.
        /// @param y Top left point y position.
        /// @param pen Which pen is used for lines.
        /// @param borderRadius What is the radius of the border.
        CellGraphicsItem(int sizex, int sizey, qreal x, qreal y, QPen* pen, int borderRadius);
        /// Get the bounding rectangle of the object.
        /// @return Rectangle that is bounding the item.
        QRectF boundingRect() const override;
        /// Paint the object to its painter.
        /// @param painter Which painter to use to paint the object.
        /// @param option Option for the Qt framework (not used).
        /// @param widget Widget for the Qt framework (not used).
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
        /// Get the shape of the item.
        /// @return Get the shape to paint.
        QPainterPath shape() const override;
    private:
        /// Border radius.
        int borderRadius_;
        /// Pointer to the used pen.
        QPen* pen_;
        /// Size of the cell in the x axis.
        int sizex_;
        /// Size of the cell in the y axis.
        int sizey_;
        /// Top left point x position.
        qreal x_;
        /// Top left point y position.
        qreal y_;
};

/// Class for creating QGraphicsView with using wheel for zoom in and out. And grabing by mouse moves the view.
class GraphicsView : public QGraphicsView {
    public:
        /// Default constructor.
        /// @param scene Which scene it is using.
        /// @param parent Which widget will be showing the view.
        explicit GraphicsView(QGraphicsScene* scene, QWidget* parent = nullptr);
    protected:
        /// Override the wheel event for scrolling.
        /// @param event When scrolling occurrred.
        void wheelEvent(QWheelEvent* event) override;
};

/// Class for showing relation suggestion and holding the data.
class RelationItem : public QTreeWidgetItem{
    public:
        /// Default constructor.
        /// @param FT Pointer to the family tree.
        /// @param suggestion Which suggestion this item represents.
        /// @param templ Pointer to the used template.
        RelationItem(FamilyTree* FT, const RelationSuggestion& suggestion, RelationTemplate* templ);
        /// Get the pointer to the relation it represents.
        /// @return Pointer to the relation.
        Relation* getRelation();
        /// Update the texts.
        void refreshTexts();
    private:
        /// Pointer to the family tree.
        FamilyTree* FT_;
        /// Which relation it represents.
        Relation relation_;
};

#endif
