/*
 * app.js - openFrameworks Emscripten (pongasoft GLFW 3.4 port)
 * Updated April 2026 for CMake migration + ofxAppEmscriptenWindow
 */

document.addEventListener('DOMContentLoaded', () => {
    console.log('DOM fully loaded - openFrameworks Emscripten runtime');

    const canvas = document.getElementById('canvas');
    if (!canvas) {
        console.error("Canvas element not found!");
        return;
    }

    // Prevent right-click context menu on canvas (common OF request)
    canvas.addEventListener('contextmenu', (event) => {
        event.preventDefault();
    });

    // Minimal Module configuration (Emscripten runtime glue)
    var statusElement = document.getElementById("status");
    var progressElement = document.getElementById("progress");
    var spinnerElement = document.getElementById("spinner");

    window.Module = {
        print: (function () {
            var element = document.getElementById("output");
            if (element) element.value = "";
            return (...args) => {
                var text = args.join(" ");
                console.log(text);
                if (element) {
                    element.value += text + "\n";
                    element.scrollTop = element.scrollHeight;
                }
            };
        })(),

        canvas: (() => {
            canvas.addEventListener("webglcontextlost", (e) => {
                alert("WebGL context lost. You will need to reload the page.");
                e.preventDefault();
            }, false);
            return canvas;
        })(),

        setStatus: (text) => {
            Module.setStatus.last ??= { time: Date.now(), text: "" };
            if (text === Module.setStatus.last.text) return;
            var m = text.match(/([^(]+)\((\d+(\.\d+)?)\/(\d+)\)/);
            var now = Date.now();
            if (m && now - Module.setStatus.last.time < 30) return;
            Module.setStatus.last.time = now;
            Module.setStatus.last.text = text;

            if (m) {
                text = m[1];
                progressElement.value = parseInt(m[2]) * 100;
                progressElement.max = parseInt(m[4]) * 100;
                progressElement.hidden = false;
                spinnerElement.hidden = false;
            } else {
                progressElement.hidden = true;
                if (!text) spinnerElement.style.display = "none";
            }
            if (statusElement) statusElement.innerHTML = text;
        },

        monitorRunDependencies: (left) => {
            Module.setStatus(left
                ? `Preparing... (${Module.totalDependencies - left}/${Module.totalDependencies})`
                : "All downloads complete."
            );
        },

        totalDependencies: 0
    };

    Module.setStatus("Downloading...");

    // Fullscreen button (now uses GLFW-aware API from ofxAppEmscriptenWindow)
    const fullscreenButton = document.getElementById('fullscreenButton');
    if (fullscreenButton) {
        fullscreenButton.addEventListener('click', () => {
            const pointerLock = document.getElementById('pointerLock')?.checked ?? false;
            const resize = document.getElementById('resize')?.checked ?? true;

            if (typeof Module.requestFullscreen === 'function') {
                Module.requestFullscreen(pointerLock, resize);
            } else if (typeof emscripten_glfw_request_fullscreen === 'function') {
                // fallback for newer GLFW port
                emscripten_glfw_request_fullscreen(pointerLock, resize);
            } else {
                console.error('Fullscreen API not available');
            }
        });
    }

    window.onerror = (text) => {
        console.error("Emscripten runtime error:", text);
        Module.setStatus("Exception thrown, see JavaScript console");
        if (spinnerElement) spinnerElement.style.display = "none";
    };
});
