const {
    app,
    BrowserWindow,
    ipcMain
} = require("electron")

const desktopframe = require("../js/index.js");

const path = require("path")

let mainWindow = null;
let embedWindow = null;

function startMainWindow() {
    mainWindow = new BrowserWindow({
        width: 800,
        height: 600,
        show: true
    });

    mainWindow.loadFile(path.join(__dirname, "index.html"));

    ipcMain.on("start", () => {
        startEmbed();
    });

    ipcMain.on("stop", () => {
        stopEmbed();
    });
}


function startEmbed() {
    embedWindow = new BrowserWindow({
        show: false,
        frame: false
    });

    embedWindow.loadURL("https://google.com");

    embedWindow.on("ready-to-show", () => {
        desktopframe.setAsDesktopFrame(embedWindow.getNativeWindowHandle().readUInt32LE(0));
        embedWindow.show();
    })

    embedWindow.on("closed", () => {
        embedWindow = null;
    })

}

function stopEmbed() {
    if(embedWindow){
        desktopframe.clearDesktopFrame(embedWindow.getNativeWindowHandle().readUInt32LE(0));
        embedWindow.close();
    }
}

app.on("ready", () => {
    startMainWindow();
});

app.on("window-all-closed", () => {

    desktopframe.clearDesktopFrame();
    app.quit();
});