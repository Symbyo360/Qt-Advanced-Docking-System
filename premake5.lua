ads = path.getabsolute(".")

function use_ads()
	includedirs "%{ads}/src"
	links "ads"
end

project "ads"
	language "C++"
	kind "SharedLib"

	files
	{
		"src/**.h",
		"src/DockAreaTabBar.cpp",
		"src/DockAreaTitleBar.cpp",
		"src/DockAreaWidget.cpp",
		"src/DockContainerWidget.cpp",
		"src/DockManager.cpp",
		"src/DockOverlay.cpp",
		"src/DockSplitter.cpp",
		"src/DockWidget.cpp",
		"src/DockWidgetTab.cpp",
		"src/ElidingLabel.cpp",
		"src/FloatingDockContainer.cpp",
		"src/ads_globals.cpp",
		"src/ads.qrc"
	}

	includedirs
	{
		"src/",
		"generated/"
	}

	-- Qt section
	qt.enable()
	local qt_path = os.getenv("QT5_PATH")
	if qt_path == nil then
		premake.warn("Qt is not found on your system so please install qt to build the qt client and add `QT5_PATH` environment variable")
		premake.warn("Don't forget to add Qt5 binary directory to your path for the client to run")
	else
		qtpath(os.getenv("QT5_PATH"))
	end
	qtprefix "Qt5"
	qtmodules { "core", "gui", "widgets" }
	qtgenerateddir "generated"

	filter "system:linux"
		os.mkdir("generated")

	filter "configurations:debug"
		qtsuffix "d"
		defines {"ADS_SHARED_EXPORT"}

	filter "configurations:release"
		defines {"ADS_SHARED_EXPORT"}

	filter {}
	