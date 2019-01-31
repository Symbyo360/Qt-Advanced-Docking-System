#ifndef DockContainerWidgetH
#define DockContainerWidgetH
/*******************************************************************************
** Qt Advanced Docking System
** Copyright (C) 2017 Uwe Kindler
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/


//============================================================================
/// \file   DockContainerWidget.h
/// \author Uwe Kindler
/// \date   24.02.2017
/// \brief  Declaration of CDockContainerWidget class
//============================================================================


//============================================================================
//                                   INCLUDES
//============================================================================
#include <QFrame>
#include <QtCore/QPointer>

#include "ads_globals.h"
#include "DockWidget.h"

class QXmlStreamWriter;
class QXmlStreamReader;
class QGridLayout;

namespace ads
{
class CDockAreaWidget;
class CDockWidget;
class CDockManager;
struct DockManagerPrivate;
class CFloatingDockContainer;
struct FloatingDockContainerPrivate;

/**
 * Private data class of CDockContainerWidget class (pimpl)
 */
class DockContainerWidgetPrivate
{
public:
	CDockContainerWidget* _this;
	QPointer<CDockManager> DockManager;
	unsigned int zOrderIndex = 0;
	QList<CDockAreaWidget*> DockAreas;
	QGridLayout* Layout = nullptr;
	QSplitter* RootSplitter = nullptr;
	bool isFloating = false;
	CDockAreaWidget* LastAddedAreaCache[5]{ 0, 0, 0, 0, 0 };
	int VisibleDockAreaCount = -1;
	CDockAreaWidget* TopLevelDockArea = nullptr;

	/**
	 * Private data constructor
	 */
	DockContainerWidgetPrivate(CDockContainerWidget* _public);

	/**
	 * Adds dock widget to container and returns the dock area that contains
	 * the inserted dock widget
	 */
	CDockAreaWidget* dockWidgetIntoContainer(DockWidgetArea area, CDockWidget* Dockwidget);

	/**
	 * Adds dock widget to a existing DockWidgetArea
	 */
	CDockAreaWidget* dockWidgetIntoDockArea(DockWidgetArea area, CDockWidget* Dockwidget,
		CDockAreaWidget* TargetDockArea);

	/**
	 * Add dock area to this container
	 */
	void addDockArea(CDockAreaWidget* NewDockWidget, DockWidgetArea area = CenterDockWidgetArea);

	/**
	 * Drop floating widget into container
	 */
	void dropIntoContainer(CFloatingDockContainer* FloatingWidget, DockWidgetArea area);

	/**
	 * Drop floating widget into dock area
	 */
	void dropIntoSection(CFloatingDockContainer* FloatingWidget,
		CDockAreaWidget* TargetArea, DockWidgetArea area);

	/**
	 * Creates a new tab for a widget dropped into the center of a section
	 */
	void dropIntoCenterOfSection(CFloatingDockContainer* FloatingWidget,
		CDockAreaWidget* TargetArea);

	/**
	 * Adds new dock areas to the internal dock area list
	 */
	void addDockAreasToList(const QList<CDockAreaWidget*> NewDockAreas);

	/**
	 * Wrapper function for DockAreas append, that ensures that dock area signals
	 * are properly connected to dock container slots
	 */
	void appendDockAreas(const QList<CDockAreaWidget*> NewDockAreas);

	/**
	 * Save state of child nodes
	 */
	void saveChildNodesState(QXmlStreamWriter& Stream, QWidget* Widget);

	/**
	 * Restore state of child nodes.
	 * \param[in] Stream The data stream that contains the serialized state
	 * \param[out] CreatedWidget The widget created from parsed data or 0 if
	 * the parsed widget was an empty splitter
	 * \param[in] Testing If Testing is true, only the stream data is
	 * parsed without modifiying anything.
	 */
	bool restoreChildNodes(QXmlStreamReader& Stream, QWidget*& CreatedWidget,
		bool Testing);

	/**
	 * Restores a splitter.
	 * \see restoreChildNodes() for details
	 */
	bool restoreSplitter(QXmlStreamReader& Stream, QWidget*& CreatedWidget,
		bool Testing);

	/**
	 * Restores a dock area.
	 * \see restoreChildNodes() for details
	 */
	bool restoreDockArea(QXmlStreamReader& Stream, QWidget*& CreatedWidget,
		bool Testing);

	/**
	 * Helper function for recursive dumping of layout
	 */
	void dumpRecursive(int level, QWidget* widget);

	/**
	 * Initializes the visible dock area count variable if it is not initialized
	 * yet
	 */
	void initVisibleDockAreaCount();

	/**
	 * Access function for the visible dock area counter
	 */
	int& visibleDockAreaCount();

	/**
	 * The visible dock area count changes, if dock areas are remove, added or
	 * when its view is toggled
	 */
	void onVisibleDockAreaCountChanged();

	void emitDockAreasRemoved();

	void emitDockAreasAdded();

	/**
	 * Helper function for creation of new splitter
	 */
	CDockSplitter* newSplitter(Qt::Orientation orientation, QWidget* parent = 0);


	// private slots: ------------------------------------------------------------
	void onDockAreaViewToggled(bool Visible);
}; // struct DockContainerWidgetPrivate

/**
 * Container that manages a number of dock areas with single dock widgets
 * or tabyfied dock widgets in each area
 */
class ADS_EXPORT CDockContainerWidget : public QFrame
{
	Q_OBJECT
private:
	DockContainerWidgetPrivate* d; ///< private data (pimpl)
    friend class DockContainerWidgetPrivate;
	friend class CDockManager;
	friend struct DockManagerPrivate;
	friend class CDockAreaWidget;
	friend struct DockAreaWidgetPrivate;
	friend class CFloatingDockContainer;
	friend struct FloatingDockContainerPrivate;
	friend class CDockWidget;
	Q_PRIVATE_SLOT(d, void onDockAreaViewToggled(bool Visible))

protected:
	/**
	 * Handles activation events to update zOrderIndex
	 */
	virtual bool event(QEvent *e) override;

	/**
	 * Access function for the internal root splitter
	 */
	QSplitter* rootSplitter() const;

	/**
	 * Helper function for creation of the root splitter
	 */
	void createRootSplitter();

	/**
	 * Drop floating widget into the container
	 */
	void dropFloatingWidget(CFloatingDockContainer* FloatingWidget, const QPoint& TargetPos);

	/**
	 * Adds the given dock area to this container widget
	 */
	void addDockArea(CDockAreaWidget* DockAreaWidget, DockWidgetArea area = CenterDockWidgetArea);

	/**
	 * Removes the given dock area from this container
	 */
	void removeDockArea(CDockAreaWidget* area);

	/**
	 * Saves the state into the given stream
	 */
	void saveState(QXmlStreamWriter& Stream) const;

	/**
	 * Restores the state from given stream.
	 * If Testing is true, the function only parses the data from the given
	 * stream but does not restore anything. You can use this check for
	 * faulty files before you start restoring the state
	 */
	bool restoreState(QXmlStreamReader& Stream, bool Testing);

	/**
	 * This function returns the last added dock area widget for the given
	 * area identifier or 0 if no dock area widget has been added for the given
	 * area
	 */
	CDockAreaWidget* lastAddedDockAreaWidget(DockWidgetArea area) const;

	/**
	 * This function returns true if this dock area has only one single
	 * visible dock widget.
	 * A top level widget is a real floating widget. Only the isFloating()
	 * function of top level widgets may returns true.
	 */
	bool hasTopLevelDockWidget() const;

	/**
	 * If hasSingleVisibleDockWidget() returns true, this function returns the
	 * one and only visible dock widget. Otherwise it returns a nullptr.
	 */
	CDockWidget* topLevelDockWidget() const;

	/**
	 * Returns the top level dock area.
	 */
	CDockAreaWidget* topLevelDockArea() const;

    /**
     * This function returns a list of all dock widgets in this floating widget.
     * It may be possible, depending on the implementation, that dock widgets,
     * that are not visible to the user have no parent widget. Therefore simply
     * calling findChildren() would not work here. Therefore this function
     * iterates over all dock areas and creates a list that contains all
     * dock widgets returned from all dock areas.
     */
    QList<CDockWidget*> dockWidgets() const;

public:
	/**
	 * Default Constructor
	 */
	CDockContainerWidget(CDockManager* DockManager, QWidget* parent = 0);

	/**
	 * Virtual Destructor
	 */
	virtual ~CDockContainerWidget();

	/**
	 * Adds dockwidget into the given area.
	 * If DockAreaWidget is not null, then the area parameter indicates the area
	 * into the DockAreaWidget. If DockAreaWidget is null, the Dockwidget will
	 * be dropped into the container.
	 * \return Returns the dock area widget that contains the new DockWidget
	 */
	CDockAreaWidget* addDockWidget(DockWidgetArea area, CDockWidget* Dockwidget,
		CDockAreaWidget* DockAreaWidget = nullptr);

	/**
	 * Returns the current zOrderIndex
	 */
	virtual unsigned int zOrderIndex() const;

	/**
	 * This function returns true if this container widgets z order index is
	 * higher than the index of the container widget given in Other parameter
	 */
	bool isInFrontOf(CDockContainerWidget* Other) const;

	/**
	 * Returns the dock area at teh given global position or 0 if there is no
	 * dock area at this position
	 */
	CDockAreaWidget* dockAreaAt(const QPoint& GlobalPos) const;

	/**
	 * Returns the dock area at the given Index or 0 if the index is out of
	 * range
	 */
	CDockAreaWidget* dockArea(int Index) const;

	/**
	 * Returns the list of dock areas that are not closed
	 * If all dock widgets in a dock area are closed, the dock area will be closed
	 */
	QList<CDockAreaWidget*> openedDockAreas() const;

	/**
	 * Returns the number of dock areas in this container
	 */
	int dockAreaCount() const;

	/**
	 * Returns the number of visible dock areas
	 */
	int visibleDockAreaCount() const;

	/**
	 * This function returns true, if this container is in a floating widget
	 */
	bool isFloating() const;

	/**
	 * Dumps the layout for debugging purposes
	 */
	void dumpLayout();

	/**
	 * This functions returns the dock widget features of all dock widget in
	 * this container.
	 * A bitwise and is used to combine the flags of all dock widgets. That
	 * means, if only dock widget does not support a certain flag, the whole
	 * dock are does not support the flag.
	 */
	CDockWidget::DockWidgetFeatures features() const;

	/**
	 * If this dock container is in a floating widget, this function returns
	 * the floating widget.
	 * Else, it returns a nullptr.
	 */
	CFloatingDockContainer* floatingWidget() const;

	/**
	 * Call this function to close all dock areas except the KeepOpenArea
	 */
	void closeOtherAreas(CDockAreaWidget* KeepOpenArea);

signals:
	/**
	 * This signal is emitted if one or multiple dock areas has been added to
	 * the internal list of dock areas.
	 * If multiple dock areas are inserted, this signal is emitted only once
	 */
	void dockAreasAdded();

	/**
	 * This signal is emitted if one or multiple dock areas has been removed
	 */
	void dockAreasRemoved();

	/**
	 * This signal is emitted if a dock area is opened or closed via
	 * toggleView() function
	 */
	void dockAreaViewToggled(CDockAreaWidget* DockArea, bool Open);
}; // class DockContainerWidget
} // namespace ads
//-----------------------------------------------------------------------------
#endif // DockContainerWidgetH
