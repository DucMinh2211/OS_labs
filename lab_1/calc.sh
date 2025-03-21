#!/bin/bash

ANS=0
HISTORY=() # ARRAY
HISTORY_SIZE=5 # ARRAY SIZE
HISTORY_FILE="./.calc_history" # FILE PATH

ANS_FILE="./.calc_ans" # FILE PATH

# Load ANS from file if it exists
if [ -f "$ANS_FILE" ]; then
  ANS=$(cat "$ANS_FILE")
else
  ANS=0
fi

load_history() { # load history file
  if [ -f "$HISTORY_FILE" ]; then
    HISTORY=($(cat "$HISTORY_FILE"))
  fi
}

save_history() { # save history to file
  echo "${HISTORY[@]}" > "$HISTORY_FILE" 
}

calculate() {
  local input_operation="$1"
  local number_1_str operator number_2_str
  local num1 num2
  local result

  IFS=' ' read -r number_1_str operator number_2_str <<< "$input_operation"

  if [[ "$number_1_str" == "ANS" ]]; then
    num1="$ANS"
  else
    num1="$number_1_str"
  fi

  if [[ "$number_2_str" == "ANS" ]]; then
    num2="$ANS"
  else
    num2="$number_2_str"
  fi

  case "$operator" in
    "+")
      result=$((num1 + num2))
      ;;
    "-")
      result=$((num1 - num2))
      ;;
    "x")
      result=$((num1 * num2))
      ;;
    "/")
      if (( num2 == 0 )); then
        echo "MATH ERROR"
        return 1
      else
        result=$(awk "BEGIN {printf \"%.2f\", $num1/$num2}") # Float division with awk
      fi
      ;;
    "%")
      if (( num2 == 0 )); then
        echo "MATH ERROR!"
        return 1
      else
        result=$((num1 % num2))
      fi
      ;;
    *)
      echo "SYNTAX ERROR"
      return 1
      ;;
  esac

  if [ $? -eq 0 ]; then
    ANS="$result"
    calculation_entry="$number_1_str $operator $number_2_str = $ANS"

    # Add calculation to history, maintaining max size
    HISTORY+=("$calculation_entry")
    if (( ${#HISTORY[@]} > HISTORY_SIZE )); then
      HISTORY=("${HISTORY[@]:1}") # Remove the oldest entry if history exceeds max size
    fi

    echo $ANS

    echo "$ANS" > "$ANS_FILE" # Save ANS to file
    save_history
  fi
}

display_history() {
  if [[ ${#HISTORY[@]} -eq 0 ]]; then
    echo "No calculation history yet."
  else
    for entry in "${HISTORY[@]}"; do
      echo "$entry"
    done
  fi
}

input=""

# load_history # Optional: Load history from file on start (not used in this version)

while [[ "$input" != "EXIT" ]]; do
  read -p ">> " input
  case "$input" in
    "EXIT")
      break
      ;;
    "HIST")
      display_history
      ;;
    *)
      calculate "$input"
      ;;
  esac
done

# save_history # Optional: Save history to file on exit (not used in this version)