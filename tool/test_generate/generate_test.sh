#!/bin/sh

TEMPLATE_FILE=test.template.json

SCHEMA_FILE="$1"
SRC_ASM_FILE="$2"
OUTPUT_DIR="$3"

if [ ! -e "${SCHEMA_FILE}" ]; then
    echo "Schema file not found. (file: ${SCHEMA_FILE})"
    exit 1
fi

if [ ! -e "${SRC_ASM_FILE}" ]; then
    echo "Assembly file not found. (file: ${SRC_ASM_FILE})"
    exit 1
fi

RELATIVE_SCHEMA_PATH=$(realpath --relative-to="${OUTPUT_DIR}" "${SCHEMA_FILE}")

LABELS=$(grep '^\s*\.proc\s' "${SRC_ASM_FILE}" | sed -e 's/^\s*\.proc\s*//g' -e 's/\s.*//g')

mkdir -p "${OUTPUT_DIR}"

for LABEL in $LABELS; do
    TEST_FILE="${OUTPUT_DIR}/${LABEL}.test.json"

    if [ -e "${TEST_FILE}" ]; then
        echo "Test file found. skip: (file: ${TEST_FILE})"
        continue
    fi

    export schema="\$schema"
    export SCHEMA="${RELATIVE_SCHEMA_PATH}"
    export LABEL="${LABEL}"
    envsubst <"${TEMPLATE_FILE}" >"${TEST_FILE}"
done
