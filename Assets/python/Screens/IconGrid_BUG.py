
from CvPythonExtensions import *
import CvUtil
import ScreenInput
import CvScreenEnums
import math


# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

def BUGPrint (stuff):
#	stuff = "BUG_IconGrid: " + stuff
#	print stuff
	return

class IconData:

	def __init__(self, sImage, iSize, widgetType, iData):
		self.image = sImage
		self.widgetType = widgetType
		self.data = iData
		self.size = iSize

class StackedBarData:

	def __init__(self, fValue, sColor, sText, iFont):
		self.value = fValue
		self.color = sColor
		self.text = sText
		self.font = iFont

class CellData:

	def __init__(self):
		self.icons = []
		self.stackedbar = []
		self.text = ""

	def addIcon(self, sImage, iSize, widgetType, iData):
		self.icons.append(IconData(sImage, iSize, widgetType, iData))

	def addText(self, sText, iFont):
		self.text = sText
		self.font = iFont

	def addStackedBar(self, fValue, sColor, sText, iFont):
		self.stackedbar.append(StackedBarData(fValue, sColor, sText, iFont))



class RowData:

	def __init__(self, sRowHeader, sMessage, iFont, iNumColumns):
		self.rowHeader = sRowHeader
		self.message = sMessage
		self.font = iFont
		self.cells = []
		for i in range(iNumColumns):
			self.cells.append(CellData())

	def addIcon(self, iColumnIndex, sImage, iSize, widgetType, iData):
		self.cells[iColumnIndex].addIcon(sImage, iSize, widgetType, iData)

	def addText(self, iColumnIndex, sText, iFont):
		self.cells[iColumnIndex].addText(sText, iFont)

	def addStackedBar(self, iColumnIndex, fValue, sColor, sText, iFont):
		self.cells[iColumnIndex].addStackedBar(fValue, sColor, sText, iFont)




class ColumnGroup:

	def __init__(self, sLabel, iLength):
		self.label = sLabel
		self.length = iLength

GRID_ICON_COLUMN = 0
GRID_MULTI_LIST_COLUMN = 1
GRID_TEXT_COLUMN = 2
GRID_STACKEDBAR_COLUMN = 3

class IconGrid_BUG:

	def __init__(self, sWidgetId, screen, iX, iY, iWidth, iHeight, columns, bUseSmallIcons, bShowRowHeader, bShowRowBorder):
		self.WIDGET_ID = sWidgetId
		self.NEXT_WIDGET_ID = sWidgetId + "IG"
		self.widgetCount = 0
		self.screen = screen
		self.xStart = iX
		self.yStart = iY
		self.width = iWidth
		self.height = iHeight
		self.columns = columns
		self.useSmallIcons = bUseSmallIcons
		self.showRowHeader = bShowRowHeader
		self.showRowBorder = bShowRowBorder
		self.columnGroups = []
		self.header = []
		self.headerFont = []
		self.data = []
		self.scrollPosition = 0
		self.textColWidth = {}
		self.StackedBarColWidth = {}

		for i in range(len(self.columns)):
			self.header.append("")
			self.headerFont.append(3)

		self.groupTitleHeight = 24
		self.headerHeight = 24
		self.iconColWidth = 55
		self.rowHeight = 68
		self.rowHeaderHeight = 17
		self.scrollArrowSize = 24
		self.rowBorderWidth = 3
		self.scrollSpace = 3

		self.groupBorder = 10
		self.groupLabelOffset = "  "
		self.minColSpace = 3
		self.minRowSpace = 0

		self.SCROLL_UP = 1
		self.SCROLL_DOWN = 2


	def setGroupBorder(self, iVal):
		self.groupBorder = iVal


	def setGroupLabelOffset(self, sVal):
		self.groupLabelOffset = sVal


	def setMinColumnSpace(self, iVal):
		self.minColSpace = iVal


	def setMinRowSpace(self, iVal):
		self.minRowSpace = iVal


	def setSize(self, iWidth, iHeight):
		self.width = iWidth
		self.height = iHeight


	def setPosition(self, iX, iY):
		self.xStart = iX
		self.yStart = iY


	def getPrefferedWidth(self):
		self.calculateLayout()

		prefferedWidth = self.scrollArrowSize + self.scrollSpace + self.minColSpace * (len(self.columns) - 1)
		for colGroup in self.columnGroups:
			if (colGroup.label != ""):
				prefferedWidth += self.groupBorder * 2

		for index in range(len(self.columns)):
			if (self.columns[index] == GRID_ICON_COLUMN):
				prefferedWidth += self.iconColWidth
			elif (self.columns[index] == GRID_MULTI_LIST_COLUMN):
				prefferedWidth += self.multiListColWidth
			elif (self.columns[index] == GRID_TEXT_COLUMN):
				prefferedWidth += self.textColWidth[index]
			elif (self.columns[index] == GRID_STACKEDBAR_COLUMN):
				prefferedWidth += self.StackedBarColWidth[index]

		if (self.showRowBorder):
			prefferedWidth += self.rowBorderWidth * 2
		return prefferedWidth



	def getPrefferedHeight(self):
		self.calculateLayout()
		initHeight = self.totalRowHeight * self.numRows + self.minRowSpace * (self.numRows - 1)

		if (len(self.columnGroups) > 0):
			if (not self.showRowHeader):
				return initHeight + self.colGroupHeight + 10
			else:
				return initHeight + self.colGroupHeight + 5
		else:
			return initHeight + self.headerHeight


	def setHeader(self, iCol, sLabel, iFont):
		self.header[iCol] = sLabel
		self.headerFont[iCol] = iFont


	def setTextColWidth(self, iCol, iWidth):
		self.textColWidth[iCol] = iWidth

	def setStackedBarColWidth(self, iCol, iWidth):
		self.StackedBarColWidth[iCol] = iWidth

	def createColumnGroup(self, sLabel, iLength):
		self.columnGroups.append(ColumnGroup(sLabel, iLength))


	def createGrid(self):
		self.calculateLayout()
		self.addControls()

	def hideGrid(self):
		self.widgetCount = 0
		self.hideControls()


	def appendRow(self, sRowHeader, sMessage, iFont):
		self.data.append(RowData(sRowHeader, sMessage, iFont, len(self.columns)))

	def addIcon(self, iRowIndex, iColumnIndex, sImage, iSize, widgetType, iData):
		self.data[iRowIndex].addIcon(iColumnIndex, sImage, iSize, widgetType, iData)

	def addText(self, iRowIndex, iColumnIndex, sText, iFont):
		self.data[iRowIndex].addText(iColumnIndex, sText, iFont)

	def addStackedBar(self, iRowIndex, iColumnIndex, fValue, sColor, sText, iFont):
#		BUGPrint("addStackedbar %i %i %s %s %i" % (iColumnIndex, fValue, sColor, sText, iFont))
		self.data[iRowIndex].addStackedBar(iColumnIndex, fValue, sColor, sText, iFont)

	def clearData(self):
		self.scrollPosition = 0
		self.data = []


	def scrollUp(self):
		self.scrollPosition = self.scrollPosition - 1
		self.refresh()


	def scrollDown(self):
		self.scrollPosition = self.scrollPosition + 1
		self.refresh()



	def refresh(self):
		# check if scrollPosistion is valid and show/hide scroll buttons
		if (self.scrollPosition <= 0):
			self.screen.hide(self.scrollUpArrow)
			self.scrollPosition = 0
		else:
			self.screen.show(self.scrollUpArrow)

		if (len(self.data) - self.numRows <= 0):
			self.screen.hide(self.scrollDownArrow)
		elif (self.scrollPosition >= len(self.data) - self.numRows):
			self.screen.hide(self.scrollDownArrow)
			self.scrollPosition = len(self.data) - self.numRows
		else:
			self.screen.show(self.scrollDownArrow)

		maxIndex = min(self.numRows, len(self.data))
		for rowIndex in range(maxIndex):
			rowData = self.data[self.scrollPosition + rowIndex]

			currentX = self.xStart
			currentY = self.firstRowY + (self.totalRowHeight + self.rowSpace) * rowIndex + 2
			if (self.showRowBorder):
				currentX += self.rowBorderWidth
				currentY += self.rowBorderWidth + 1
			if (self.showRowHeader):
				currentY += self.rowHeaderHeight
				text = "<font=%i>%s</font>" % (rowData.font, rowData.rowHeader)
				self.screen.setLabel(self.rowName + str(rowIndex) + "name", "",
									 text, CvUtil.FONT_LEFT_JUSTIFY,
									 self.xStart + 5, self.firstRowY + (self.totalRowHeight + self.rowSpace) * rowIndex - 3,
									 0, FontTypes.GAME_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1 )

			startIndex = 0
			for groupIndex in range(len(self.columnGroups)):
				colGroup = self.columnGroups[groupIndex]
				if (colGroup.label != ""):
					currentX += self.groupBorder

				# put info in grouped columns
				for offset in range(colGroup.length):
#					BUGPrint("Grouped Column %i %i" % (startIndex + offset, self.columns[startIndex + offset]))
					if (self.columns[startIndex + offset] == GRID_ICON_COLUMN):
						bDataFound = True
						try:
							iconData = rowData.cells[startIndex + offset].icons[0]
						except:
							bDataFound = False

						if bDataFound:
							self.screen.setImageButton(self.rowName + str(rowIndex) + "_" + str(startIndex + offset),
													   iconData.image, currentX - (iconData.size - 64) / 2, currentY - (iconData.size - 64) / 2, iconData.size, iconData.size,
													   iconData.widgetType, iconData.data, -1)
							currentX += self.iconColWidth + self.colSpace

					elif (self.columns[startIndex + offset] == GRID_MULTI_LIST_COLUMN):
						self.screen.clearMultiList(self.rowName + str(rowIndex) + "_" + str(startIndex + offset))
						for icon in rowData.cells[startIndex + offset].icons:
							self.screen.appendMultiListButton(self.rowName + str(rowIndex) + "_" + str(startIndex + offset),
															  icon.image, 0, icon.widgetType, icon.data, -1, False)
						currentX += self.multiListColWidth + self.colSpace

					elif (self.columns[startIndex + offset] == GRID_TEXT_COLUMN):
						textY = self.firstRowY + (self.totalRowHeight + self.rowSpace) * rowIndex + 28
						if (self.showRowHeader):
							textY += self.rowHeaderHeight
						text = "<font=%i>%s</font>" % (rowData.cells[startIndex + offset].font, rowData.cells[startIndex + offset].text)
						self.screen.setLabel( self.rowName + str(rowIndex) + "_" + str(startIndex + offset), ""
											, text, CvUtil.FONT_LEFT_JUSTIFY
											, currentX + 6, textY, 0, FontTypes.GAME_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1 )
						currentX += self.textColWidth[startIndex + offset] + self.colSpace

					elif (self.columns[startIndex + offset] == GRID_STACKEDBAR_COLUMN):
#						BUGPrint("Stacked Bar Start A")

						bDataFound = True
						try:
#							BUGPrint("Stacked Bar try")
							stackedbarData = rowData.cells[startIndex + offset].stackedbar[0]
						except:
							bDataFound = False

#						BUGPrint("Stacked Bar data found? %s" % (bDataFound))
						if bDataFound:
							textY = self.firstRowY + (self.totalRowHeight + self.rowSpace) * rowIndex + 20
							if (self.showRowHeader):
								textY += self.rowHeaderHeight

							width = self.StackedBarColWidth[startIndex + offset] - 15

							if stackedbarData.text != "" and stackedbarData.value > 0:
								iSBarOffset_Y = 10
							else:
								iSBarOffset_Y = 0

#							BUGPrint("Stacked Bar value %i" % (stackedbarData.value))
							if stackedbarData.value > 0:
								szBar_ID = self.rowName + str(rowIndex) + "_" + str(startIndex + offset) + "SB"
								self.screen.addStackedBarGFC(szBar_ID,
															 currentX + 6, textY + iSBarOffset_Y, width, 25,
															 InfoBarTypes.NUM_INFOBAR_TYPES, WidgetTypes.WIDGET_GENERAL, -1, -1 )

								self.screen.setBarPercentage(szBar_ID, InfoBarTypes.INFOBAR_STORED, float(stackedbarData.value) / float(100))
								self.screen.setStackedBarColors(szBar_ID, InfoBarTypes.INFOBAR_STORED, gc.getInfoTypeForString(stackedbarData.color))

							if stackedbarData.text != "":
								szTxt_ID = self.rowName + str(rowIndex) + "_" + str(startIndex + offset) + "T"
								text = "<font=%i>%s</font>" % (stackedbarData.font, stackedbarData.text)
								self.screen.setLabel (szTxt_ID, "",
													  text, CvUtil.FONT_CENTER_JUSTIFY,
													  currentX + 6 + width / 2, textY - iSBarOffset_Y,
													  -0.1, FontTypes.GAME_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1)

							currentX += self.StackedBarColWidth[startIndex + offset] + self.colSpace
#							BUGPrint("Stacked Bar done")

				startIndex += colGroup.length
				if (colGroup.label != ""):
					currentX += self.groupBorder

			# put info in non grouped columns
			for offset in range(len(self.columns) - startIndex):
#				BUGPrint("Single Column %i %i" % (startIndex + offset, self.columns[startIndex + offset]))
				if (self.columns[startIndex + offset] == GRID_ICON_COLUMN):
					bDataFound = True
					try:
						iconData = rowData.cells[startIndex + offset].icons[0]
					except:
						bDataFound = False

					if bDataFound:
						self.screen.setImageButton(self.rowName + str(rowIndex) + "_" + str(startIndex + offset),
												   iconData.image, currentX - (iconData.size - 64) / 2, currentY - (iconData.size - 64) / 2, iconData.size, iconData.size,
												   iconData.widgetType, iconData.data, -1 )
						currentX += self.iconColWidth + self.colSpace

				elif (self.columns[startIndex + offset] == GRID_MULTI_LIST_COLUMN):
					self.screen.clearMultiList(self.rowName + str(rowIndex) + "_" + str(startIndex + offset))
					for icon in rowData.cells[startIndex + offset].icons:
						self.screen.appendMultiListButton( self.rowName + str(rowIndex) + "_" + str(startIndex + offset)
														 , icon.image, 0, icon.widgetType, icon.data, -1, False )
					currentX += self.multiListColWidth + self.colSpace

				elif (self.columns[startIndex + offset] == GRID_TEXT_COLUMN):
					text = "<font=%i>%s</font>" % (rowData.cells[startIndex + offset].font, rowData.cells[startIndex + offset].text)
					self.screen.setLabel(self.rowName + str(rowIndex) + "_" + str(startIndex + offset), "",
										 text, CvUtil.FONT_LEFT_JUSTIFY,
										 currentX + 6, self.firstRowY + (self.totalRowHeight + self.rowSpace) * rowIndex + 28,
										 0, FontTypes.GAME_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1 )
					currentX += self.textColWidth[startIndex + offset] + self.colSpace

				elif (self.columns[startIndex + offset] == GRID_STACKEDBAR_COLUMN):
#					BUGPrint("Stacked Bar Start B")
					bDataFound = True
					try:
						stackedbarData = rowData.cells[startIndex + offset].stackedbar[0]
					except:
						bDataFound = False

					if bDataFound:
						textY = self.firstRowY + (self.totalRowHeight + self.rowSpace) * rowIndex + 20

						width = self.StackedBarColWidth[startIndex + offset] - 15

						if stackedbarData.text != "" and stackedbarData.value > 0:
							iSBarOffset_Y = 10
						else:
							iSBarOffset_Y = 0

#						BUGPrint("Stacked Bar value %i" % (stackedbarData.value))
						if stackedbarData.value > 0:
							szBar_ID = self.rowName + str(rowIndex) + "_" + str(startIndex + offset) + "SB"
							self.screen.addStackedBarGFC(szBar_ID,
														 currentX + 6, textY + iSBarOffset_Y, width, 25,
														 InfoBarTypes.NUM_INFOBAR_TYPES, WidgetTypes.WIDGET_GENERAL, -1, -1 )
							self.screen.setBarPercentage(szBar_ID, InfoBarTypes.INFOBAR_STORED, float(stackedbarData.value) / float(100))
							self.screen.setStackedBarColors(szBar_ID, InfoBarTypes.INFOBAR_STORED, gc.getInfoTypeForString(stackedbarData.color))

						if stackedbarData.text != "":
							szTxt_ID = self.rowName + str(rowIndex) + "_" + str(startIndex + offset) + "T"
							text = "<font=%i>%s</font>" % (stackedbarData.font, stackedbarData.text)
							self.screen.setLabel (szTxt_ID, "",
												  text, CvUtil.FONT_CENTER_JUSTIFY,
												  currentX + 6 + width / 2, textY - iSBarOffset_Y,
												  -0.1, FontTypes.GAME_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1)

						currentX += self.StackedBarColWidth[startIndex + offset] + self.colSpace
#						BUGPrint("Stacked Bar done")

			if ( rowData.message == "" ):
				self.screen.attachLabel(self.rowName + str(rowIndex), self.rowName + str(rowIndex) + "NotConnected", "")
			else:
				text = "<font=%i>                              %s</font>" % (rowData.font, rowData.message)
				self.screen.attachLabel(self.rowName + str(rowIndex),
										self.rowName + str(rowIndex) + "NotConnected",
										text)




	def calculateLayout(self):
		if (self.useSmallIcons):
			self.iconSize = 32
		else:
			self.iconSize = 64
		# width
		availableWidth = self.width - self.scrollArrowSize - self.scrollSpace - self.minColSpace * (len(self.columns) - 1)
		useColGroups = False
		for colGroup in self.columnGroups:
			if (colGroup.label != ""):
				availableWidth -= self.groupBorder * 2
				useColGroups = True

		numMultiListCols = 0
		for colIndex in range(len(self.columns)):
			col = self.columns[colIndex]
			if (col == GRID_ICON_COLUMN):
				availableWidth -= self.iconColWidth
			if (col == GRID_MULTI_LIST_COLUMN):
				numMultiListCols += 1
			if (col == GRID_TEXT_COLUMN):
				availableWidth -= self.textColWidth[colIndex]
			if (col == GRID_STACKEDBAR_COLUMN):
				availableWidth -= self.StackedBarColWidth[colIndex]

		if (self.showRowBorder):
			availableWidth -= self.rowBorderWidth * 2

		initMultiListColWidth = availableWidth / numMultiListCols
		numIcons = (initMultiListColWidth - 16) / (self.iconSize + 2)
		self.multiListColWidth = numIcons * (self.iconSize + 2) + 16
		self.colSpace = (availableWidth - self.multiListColWidth * numMultiListCols) / (len(self.columns) - 1) + self.minColSpace

		# height
		if (useColGroups):
			self.colGroupHeight = self.groupTitleHeight + self.headerHeight + 8
			self.headerY = self.yStart + self.groupTitleHeight + 3
			self.firstRowY = self.headerY + self.headerHeight + 7
			availableHeight = self.height - self.colGroupHeight - 5
			if (not self.showRowHeader):
				self.firstRowY += 5
				availableHeight -= 5
		else:
			self.colCroupHeight = 0
			self.headerY = self.yStart
			self.firstRowY = self.headerY + self.headerHeight
			availableHeight = self.height - self.headerHeight

		if (self.showRowHeader):
			self.totalRowHeight = self.rowHeight + self.rowHeaderHeight
			self.multiListStartY = self.firstRowY + self.rowHeaderHeight
		else:
			self.totalRowHeight = self.rowHeight
			self.multiListStartY = self.firstRowY

		if (self.showRowBorder):
			self.totalRowHeight += self.rowBorderWidth * 2

		self.numRows = (availableHeight + self.minRowSpace) / (self.totalRowHeight + self.minRowSpace)
		self.rowSpace = (availableHeight - self.numRows * self.totalRowHeight) / (self.numRows - 1)





	def addControls(self):
		self.addGroups()
		self.addHeader()

		# add the rows
		self.leaderHeadName = self.getNextWidgetName()
		self.multiListName = self.getNextWidgetName()
		self.rowName = self.getNextWidgetName()

		for rowIndex in range(self.numRows):
			self.addRow(rowIndex)

		self.scrollUpArrow = self.getNextWidgetName()
		self.scrollDownArrow = self.getNextWidgetName()
		self.screen.setImageButton( self.scrollUpArrow
								  , ArtFileMgr.getInterfaceArtInfo("SCROLL_UP_ARROW").getPath()
								  , self.xStart + self.width - self.scrollArrowSize
								  , self.firstRowY
								  , self.scrollArrowSize, self.scrollArrowSize
								  , WidgetTypes.WIDGET_GENERAL, self.SCROLL_UP, -1 )
		self.screen.setImageButton( self.scrollDownArrow
								  , ArtFileMgr.getInterfaceArtInfo("SCROLL_DOWN_ARROW").getPath()
								  , self.xStart + self.width - self.scrollArrowSize
								  , self.yStart + self.height - self.scrollArrowSize
								  , self.scrollArrowSize, self.scrollArrowSize
								  , WidgetTypes.WIDGET_GENERAL, self.SCROLL_DOWN, -1 )



	def addGroups(self):
		self.groupPanelName = self.getNextWidgetName()

		startIndex = 0
		lastColGroupIndex = 0
		colGroupX = self.xStart

		for index in range(len(self.columnGroups)):
			colGroup = self.columnGroups[index]
			if (colGroup.label != ""):
				colGroupWidth = self.groupBorder * 2
				for offset in range(colGroup.length):
					if (self.columns[startIndex + offset] == GRID_ICON_COLUMN):
						colGroupWidth += self.iconColWidth + self.colSpace
					elif (self.columns[startIndex + offset] == GRID_MULTI_LIST_COLUMN):
						colGroupWidth += self.multiListColWidth + self.colSpace
					elif (self.columns[startIndex + offset] == GRID_TEXT_COLUMN):
						colGroupWidth += self.textColWidth[startIndex + offset] + self.colSpace
					elif (self.columns[startIndex + offset] == GRID_STACKEDBAR_COLUMN):
						colGroupWidth += self.StackedBarColWidth[startIndex + offset] + self.colSpace

				colGroupWidth -= self.colSpace
				if (colGroup.length == 1):
					self.groupLabelOffset = "  "
					self.screen.addPanel( self.groupPanelName + str(index), self.groupLabelOffset + colGroup.label, "", False, False
										, colGroupX, self.yStart, colGroupWidth + 7, self.colGroupHeight
										, PanelStyles.PANEL_STYLE_EMPTY )
				else:
					self.groupLabelOffset = "  "
					self.screen.addPanel( self.groupPanelName + str(index), self.groupLabelOffset + colGroup.label, "", False, False
										, colGroupX, self.yStart, colGroupWidth + 7, self.colGroupHeight
										, PanelStyles.PANEL_STYLE_MAIN_TAN )
				colGroupX += colGroupWidth + self.colSpace
			else:
				for offset in range(colGroup.length):
					if (self.columns[startIndex + offset] == GRID_ICON_COLUMN):
						colGroupX += self.iconColWidth + self.colSpace
					elif (self.columns[startIndex + offset] == GRID_MULTI_LIST_COLUMN):
						colGroupX += self.multiListColWidth + self.colSpace
					elif (self.columns[startIndex + offset] == GRID_TEXT_COLUMN):
						colGroupX += self.textColWidth[startIndex + offset] + self.colSpace
					elif (self.columns[startIndex + offset] == GRID_STACKEDBAR_COLUMN):
						colGroupX += self.StackedBarColWidth[startIndex + offset] + self.colSpace

			startIndex += colGroup.length



	def addHeader(self):
		self.headerName = self.getNextWidgetName()
		headerX = self.xStart
		if (len(self.columnGroups) > 0 and self.columnGroups[0].label != ""):
			headerX += self.groupBorder
		self.screen.addTableControlGFC( self.headerName, len(self.columns)
									  , headerX, self.headerY, self.width - self.scrollArrowSize, self.headerHeight
									  , False, False, 16, 16, TableStyles.TABLE_STYLE_EMPTY )
		startIndex = 0
		for groupIndex in range(len(self.columnGroups)):
			colGroup = self.columnGroups[groupIndex]
			headerWidth = 0

			for offset in range(colGroup.length):
				if (self.columns[startIndex + offset] == GRID_ICON_COLUMN):
					headerWidth = self.iconColWidth + self.colSpace
				elif (self.columns[startIndex + offset] == GRID_MULTI_LIST_COLUMN):
					headerWidth = self.multiListColWidth + self.colSpace
				elif (self.columns[startIndex + offset] == GRID_TEXT_COLUMN):
					headerWidth = self.textColWidth[startIndex + offset] + self.colSpace
				elif (self.columns[startIndex + offset] == GRID_STACKEDBAR_COLUMN):
					headerWidth = self.StackedBarColWidth[startIndex + offset] + self.colSpace

				if (offset == colGroup.length - 1): # last column of this group
					if (colGroup.label != ""):
						headerWidth += self.groupBorder
					if (groupIndex < len(self.columnGroups) - 1 and self.columnGroups[groupIndex + 1].label != ""):
						headerWidth += self.groupBorder

				self.screen.setTableColumnHeader( self.headerName, startIndex + offset, "", headerWidth )
				text = "<font=%i>%s</font>" % (self.headerFont[startIndex + offset], self.header[startIndex + offset])
				self.screen.setTableText(self.headerName, startIndex + offset, 0,
										 text, "", WidgetTypes.WIDGET_GENERAL, -1, -1, 0 )
			startIndex += colGroup.length

		for offset in range(len(self.columns) - startIndex):
			if (self.columns[startIndex + offset] == GRID_ICON_COLUMN):
				headerWidth = self.iconColWidth + self.colSpace
			elif (self.columns[startIndex + offset] == GRID_MULTI_LIST_COLUMN):
				headerWidth = self.multiListColWidth + self.colSpace
			elif (self.columns[startIndex + offset] == GRID_TEXT_COLUMN):
				headerWidth = self.textColWidth[startIndex + offset] + self.colSpace
			elif (self.columns[startIndex + offset] == GRID_STACKEDBAR_COLUMN):
				headerWidth = self.StackedBarColWidth[startIndex + offset] + self.colSpace

			self.screen.setTableColumnHeader( self.headerName, startIndex + offset, "", headerWidth )
			text = "<font=%i>%s</font>" % (self.headerFont[startIndex + offset], self.header[startIndex + offset])
			self.screen.setTableText(self.headerName, startIndex + offset, 0,
									 text, "", WidgetTypes.WIDGET_GENERAL, -1, -1, 0 )




	def addRow(self, rowIndex):
		if (self.showRowBorder):
			panelY = self.firstRowY + (self.totalRowHeight + self.rowSpace) * rowIndex
			panelHeight = self.totalRowHeight
			if (self.showRowHeader):
				panelY += self.rowHeaderHeight
				panelHeight -= self.rowHeaderHeight

			self.screen.addPanel( self.rowName + str(rowIndex), "", "", False, True, self.xStart, panelY
								, self.width - self.scrollArrowSize - self.scrollSpace, panelHeight
								, PanelStyles.PANEL_STYLE_OUT )

		startIndex = 0
		currentX = self.xStart
		listY = self.multiListStartY + (self.totalRowHeight + self.rowSpace) * rowIndex
		if (self.showRowBorder):
			currentX += self.rowBorderWidth
			listY += self.rowBorderWidth + 1

		for groupIndex in range(len(self.columnGroups)):
			colGroup = self.columnGroups[groupIndex]
			if (colGroup.label != ""):
				currentX += self.groupBorder

			for offset in range(colGroup.length):
				if (self.columns[startIndex + offset] == GRID_ICON_COLUMN):
					currentX += self.iconColWidth + self.colSpace
				elif (self.columns[startIndex + offset] == GRID_MULTI_LIST_COLUMN):
					self.screen.addMultiListControlGFC( self.rowName + str(rowIndex) + "_" + str(startIndex + offset), ""
													  , currentX, listY, self.multiListColWidth, self.rowHeight
													  , 1, self.iconSize, self.iconSize, TableStyles.TABLE_STYLE_EMPTY )
					currentX += self.multiListColWidth + self.colSpace
				elif (self.columns[startIndex + offset] == GRID_TEXT_COLUMN):
					currentX += self.textColWidth[startIndex + offset] + self.colSpace
				elif (self.columns[startIndex + offset] == GRID_STACKEDBAR_COLUMN):
					currentX += self.StackedBarColWidth[startIndex + offset] + self.colSpace

			startIndex += colGroup.length
			if (colGroup.label != ""):
				currentX += self.groupBorder

		for offset in range(len(self.columns) - startIndex):
			if (self.columns[startIndex + offset] == GRID_ICON_COLUMN):
				currentX += self.iconColWidth + self.colSpace
			elif (self.columns[startIndex + offset] == GRID_MULTI_LIST_COLUMN):
				self.screen.addMultiListControlGFC( self.rowName + str(rowIndex) + "_" + str(startIndex + offset), ""
												  , currentX, listY, self.multiListColWidth, self.rowHeight
												  , 1, self.iconSize, self.iconSize, TableStyles.TABLE_STYLE_EMPTY )
				currentX += self.multiListColWidth + self.colSpace
			elif (self.columns[startIndex + offset] == GRID_TEXT_COLUMN):
				currentX += self.textColWidth[startIndex + offset] + self.colSpace
			elif (self.columns[startIndex + offset] == GRID_STACKEDBAR_COLUMN):
				currentX += self.StackedBarColWidth[startIndex + offset] + self.colSpace










	def hideControls(self):
		self.hideGroups()
		self.hideHeader()

		# add the rows
		self.leaderHeadName = self.getNextWidgetName()
		self.multiListName = self.getNextWidgetName()
		self.rowName = self.getNextWidgetName()

		for rowIndex in range(self.numRows):
			self.hideRow(rowIndex)

		self.scrollUpArrow = self.getNextWidgetName()
		self.scrollDownArrow = self.getNextWidgetName()
		self.screen.deleteWidget(self.scrollUpArrow)
		self.screen.deleteWidget(self.scrollDownArrow)

	def hideGroups(self):
		self.groupPanelName = self.getNextWidgetName()

		for index in range(len(self.columnGroups)):
			if (self.columnGroups[index].label != ""):
				self.screen.deleteWidget(self.groupPanelName + str(index))

	def hideHeader(self):
		self.headerName = self.getNextWidgetName()
		self.screen.deleteWidget(self.headerName)

	def hideRow(self, rowIndex):
		if (self.showRowBorder):
			self.screen.deleteWidget(self.rowName + str(rowIndex))

		startIndex = 0
		for groupIndex in range(len(self.columnGroups)):
			colGroup = self.columnGroups[groupIndex]

			for offset in range(colGroup.length):
				if (self.columns[startIndex + offset] == GRID_ICON_COLUMN):
					self.screen.hide( self.rowName + str(rowIndex) + "_" + str(startIndex + offset))
				elif (self.columns[startIndex + offset] == GRID_MULTI_LIST_COLUMN):
					self.screen.deleteWidget(self.rowName + str(rowIndex) + "_" + str(startIndex + offset))
				elif (self.columns[startIndex + offset] == GRID_TEXT_COLUMN):
					self.screen.hide( self.rowName + str(rowIndex) + "_" + str(startIndex + offset))
				elif (self.columns[startIndex + offset] == GRID_STACKEDBAR_COLUMN):
					self.screen.hide( self.rowName + str(rowIndex) + "_" + str(startIndex + offset))
					self.screen.hide( self.rowName + str(rowIndex) + "_" + str(startIndex + offset) + "SB")
					self.screen.hide( self.rowName + str(rowIndex) + "_" + str(startIndex + offset) + "T")

			startIndex += colGroup.length

		for offset in range(len(self.columns) - startIndex):
			if (self.columns[startIndex + offset] == GRID_ICON_COLUMN):
				self.screen.hide( self.rowName + str(rowIndex) + "_" + str(startIndex + offset))
			elif (self.columns[startIndex + offset] == GRID_MULTI_LIST_COLUMN):
				self.screen.deleteWidget(self.rowName + str(rowIndex) + "_" + str(startIndex + offset))
			elif (self.columns[startIndex + offset] == GRID_TEXT_COLUMN):
				self.screen.hide( self.rowName + str(rowIndex) + "_" + str(startIndex + offset))
			elif (self.columns[startIndex + offset] == GRID_STACKEDBAR_COLUMN):
				self.screen.hide( self.rowName + str(rowIndex) + "_" + str(startIndex + offset))
				self.screen.hide( self.rowName + str(rowIndex) + "_" + str(startIndex + offset) + "SB")
				self.screen.hide( self.rowName + str(rowIndex) + "_" + str(startIndex + offset) + "T")










	def getNextWidgetName(self):
		szName = self.NEXT_WIDGET_ID + str(self.widgetCount)
		self.widgetCount += 1
		return szName

	def deleteAllWidgets(self):
		i = self.widgetCount - 1
		while (i >= 0):
			self.widgetCount = i
			self.screen.deleteWidget(self.getNextWidgetName())
			i -= 1

		self.widgetCount = 0

		self.screen.hide(self.scrollDownArrow)
		self.screen.hide(self.scrollDownArrow)

		maxIndex = min(self.numRows, len(self.data))
		for rowIndex in range(maxIndex):
			self.screen.hide(self.rowName + str(rowIndex) + "name")

			self.screen.deleteWidget(self.rowName + str(rowIndex))









			startIndex = 0
			for groupIndex in range(len(self.columnGroups)):
				colGroup = self.columnGroups[groupIndex]
				if (colGroup.label != ""):
					currentX += self.groupBorder

				for offset in range(colGroup.length):
					if (self.columns[startIndex + offset] == GRID_ICON_COLUMN):
						self.screen.hide(self.rowName + str(rowIndex) + "_" + str(startIndex + offset))





