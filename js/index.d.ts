declare module "electron-desktop-frame" {
    export function setAsDesktopFrame(window: number): bool;
    export function clearDesktopFrame(window?: number): void;
}