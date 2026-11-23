#!/bin/sh
set -eu
: "${MQTT_USER:?MQTT_USER is not set (see .env)}"
: "${MQTT_PASSWORD:?MQTT_PASSWORD is not set (see .env)}"

umask 077
PASSWD_FILE=/mosquitto/passwd
printf '%s:%s\n' "$MQTT_USER" "$MQTT_PASSWORD" > "$PASSWD_FILE"
mosquitto_passwd -U "$PASSWD_FILE"
chown mosquitto:mosquitto "$PASSWD_FILE"

exec mosquitto -c /mosquitto/config/mosquitto.conf
