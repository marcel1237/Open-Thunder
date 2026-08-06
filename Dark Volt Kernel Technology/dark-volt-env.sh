# ============================================================
# DARK VOLT - Hardware Environment Setup
# ============================================================

# 1. Force Qt EGLFS (Direct DRM/KMS access)
# Bypasses X11/Wayland for early-boot hardware drawing.
QT_QPA_PLATFORM=eglfs

# 2. DRM/KMS Device configuration
# Hints the browser to use the primary GPU buffer directly.
QT_QPA_EGLFS_KMS_CONFIG=/etc/thunder/kms-config.json
QT_QPA_EGLFS_FORCE888=1

# 3. Input Handling (Libinput)
# Early-boot needs direct handling of mouse/keyboard events.
QT_QPA_EGLFS_DISABLE_INPUT=0
QT_QPA_FB_DISABLE_CURSOR=0

# 4. Performance Hints for EGLFS
QT_QPA_EGLFS_INTEGRATION=eglfs_kms
QT_EGLFS_NON_INTERACTIVE=0
