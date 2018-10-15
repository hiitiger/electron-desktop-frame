declare module "electron-desktop-frame" {
    export function setAsDesktopFrame(window: number, display?: number): boolean;
    export function clearDesktopFrame(window?: number): void;
}