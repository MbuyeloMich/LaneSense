#!/usr/bin/env python3
"""
UKF Highway Kalman Filter Visualization
Shows RMSE convergence and filter performance metrics
"""

import re
import os
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from matplotlib.animation import FuncAnimation
import numpy as np

def parse_output_file(filepath):
    """Parse output.txt to extract RMSE metrics"""
    if not os.path.exists(filepath):
        print(f"Error: {filepath} not found. Run the UKF app first.")
        return None, None, None
    
    with open(filepath, 'r') as f:
        content = f.read()
    
    # Extract RMSE values for each timestep
    pattern = r"Time: (\d+)s \| RMSE - X: ([\d.e+-]+), Y: ([\d.e+-]+), Vx: ([\d.e+-]+),"
    matches = re.findall(pattern, content)
    
    if not matches:
        print("Could not parse RMSE data from output file")
        return None, None, None
    
    times = []
    rmse_x = []
    rmse_y = []
    rmse_vx = []
    
    for match in matches:
        times.append(int(match[0]))
        rmse_x.append(float(match[1]))
        rmse_y.append(float(match[2]))
        rmse_vx.append(float(match[3]))
    
    # Extract final RMSE
    final_pattern = r"Final RMSE:.*?X position:\s+([\d.e+-]+).*?Y position:\s+([\d.e+-]+).*?X velocity:\s+([\d.e+-]+).*?Y velocity:\s+([\d.e+-]+)"
    final_match = re.search(final_pattern, content, re.DOTALL)
    
    final_rmse = None
    if final_match:
        final_rmse = {
            'X': float(final_match.group(1)),
            'Y': float(final_match.group(2)),
            'Vx': float(final_match.group(3)),
            'Vy': float(final_match.group(4))
        }
    
    return {
        'times': times,
        'rmse_x': rmse_x,
        'rmse_y': rmse_y,
        'rmse_vx': rmse_vx
    }, final_rmse, content

def create_visualization(data, final_rmse, content):
    """Create comprehensive visualization"""
    if data is None:
        return
    
    fig = plt.figure(figsize=(16, 12))
    fig.suptitle('Unscented Kalman Filter - Highway Vehicle Tracking', 
                 fontsize=16, fontweight='bold', y=0.98)
    
    # ========== Plot 1: RMSE over Time ==========
    ax1 = plt.subplot(2, 3, 1)
    ax1.plot(data['times'], data['rmse_x'], 'b-o', linewidth=2, markersize=6, label='X Position')
    ax1.plot(data['times'], data['rmse_y'], 'r-s', linewidth=2, markersize=6, label='Y Position')
    ax1.plot(data['times'], data['rmse_vx'], 'g-^', linewidth=2, markersize=6, label='X Velocity')
    
    # Add threshold lines
    thresholds = [0.30, 0.16, 0.95, 0.70]
    ax1.axhline(y=thresholds[0], color='b', linestyle='--', alpha=0.5, label='X Threshold (0.30)')
    ax1.axhline(y=thresholds[1], color='r', linestyle='--', alpha=0.5, label='Y Threshold (0.16)')
    ax1.axhline(y=thresholds[2], color='g', linestyle='--', alpha=0.5, label='Vx Threshold (0.95)')
    
    ax1.set_xlabel('Time (seconds)', fontsize=11, fontweight='bold')
    ax1.set_ylabel('RMSE (m or m/s)', fontsize=11, fontweight='bold')
    ax1.set_title('RMSE Convergence Over Time', fontsize=12, fontweight='bold')
    ax1.legend(fontsize=8, loc='best')
    ax1.grid(True, alpha=0.3)
    ax1.set_ylim([0, max(max(data['rmse_x']), max(data['rmse_y']), max(data['rmse_vx'])) * 1.1])
    
    # ========== Plot 2: Final RMSE Comparison ==========
    ax2 = plt.subplot(2, 3, 2)
    if final_rmse:
        metrics = ['X Position\n(m)', 'Y Position\n(m)', 'X Velocity\n(m/s)', 'Y Velocity\n(m/s)']
        values = [final_rmse['X'], final_rmse['Y'], final_rmse['Vx'], final_rmse['Vy']]
        thresholds = [0.30, 0.16, 0.95, 0.70]
        
        x_pos = np.arange(len(metrics))
        colors = ['red' if v > t else 'green' for v, t in zip(values, thresholds)]
        
        bars = ax2.bar(x_pos, values, color=colors, alpha=0.7, edgecolor='black', linewidth=2)
        
        # Add threshold lines
        ax2.plot(x_pos, thresholds, 'k--', marker='o', linewidth=2, markersize=8, label='Thresholds')
        
        # Add value labels on bars
        for i, (bar, val) in enumerate(zip(bars, values)):
            height = bar.get_height()
            ax2.text(bar.get_x() + bar.get_width()/2., height,
                    f'{val:.2f}', ha='center', va='bottom', fontweight='bold', fontsize=10)
        
        ax2.set_ylabel('RMSE Value', fontsize=11, fontweight='bold')
        ax2.set_title('Final RMSE vs Thresholds', fontsize=12, fontweight='bold')
        ax2.set_xticks(x_pos)
        ax2.set_xticklabels(metrics, fontsize=10)
        ax2.legend(fontsize=9)
        ax2.grid(True, alpha=0.3, axis='y')
    
    # ========== Plot 3: RMSE X and Y Detailed ==========
    ax3 = plt.subplot(2, 3, 3)
    width = 0.35
    x_pos = np.arange(len(data['times']))
    
    ax3.bar(x_pos - width/2, data['rmse_x'], width, label='X Position', alpha=0.8, color='steelblue')
    ax3.bar(x_pos + width/2, data['rmse_y'], width, label='Y Position', alpha=0.8, color='coral')
    
    ax3.set_xlabel('Time (seconds)', fontsize=11, fontweight='bold')
    ax3.set_ylabel('RMSE (m)', fontsize=11, fontweight='bold')
    ax3.set_title('Position Error Comparison', fontsize=12, fontweight='bold')
    ax3.set_xticks(x_pos)
    ax3.set_xticklabels(data['times'])
    ax3.legend(fontsize=10)
    ax3.grid(True, alpha=0.3, axis='y')
    
    # ========== Plot 4: Simulation Info (Text Box) ==========
    ax4 = plt.subplot(2, 3, 4)
    ax4.axis('off')
    
    # Extract simulation parameters
    duration_match = re.search(r'Duration: (\d+) seconds', content)
    timestep_match = re.search(r'Time step: ([\d.]+) seconds', content)
    ego_vel_match = re.search(r'Ego car velocity: ([\d.]+) m/s', content)
    
    info_text = "SIMULATION PARAMETERS\n" + "="*40 + "\n\n"
    if duration_match:
        info_text += f"Duration: {duration_match.group(1)} seconds\n"
    if timestep_match:
        info_text += f"Time Step: {timestep_match.group(1)} seconds\n"
    if ego_vel_match:
        info_text += f"Ego Car Velocity: {ego_vel_match.group(1)} m/s\n"
    
    info_text += f"\nNO. OF FRAMES: {data['times'][-1] * 30}\n"
    info_text += f"FRAME RATE: 30 fps\n\n"
    
    info_text += "TRACKED VEHICLES:\n"
    info_text += "  • Car 1: (-10, 4) m, 5 m/s\n"
    info_text += "  • Car 2: (25, -4) m, -6 m/s\n"
    info_text += "  • Car 3: (-12, 0) m, 1 m/s\n"
    
    ax4.text(0.05, 0.95, info_text, transform=ax4.transAxes, fontsize=10,
            verticalalignment='top', fontfamily='monospace',
            bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.5))
    
    # ========== Plot 5: RMSE Statistics ==========
    ax5 = plt.subplot(2, 3, 5)
    ax5.axis('off')
    
    stats_text = "RMSE STATISTICS\n" + "="*40 + "\n\n"
    
    stats_text += "X Position (m):\n"
    stats_text += f"  Min: {min(data['rmse_x']):.4f}\n"
    stats_text += f"  Max: {max(data['rmse_x']):.4f}\n"
    stats_text += f"  Mean: {np.mean(data['rmse_x']):.4f}\n"
    stats_text += f"  Final: {final_rmse['X']:.4f}\n\n"
    
    stats_text += "Y Position (m):\n"
    stats_text += f"  Min: {min(data['rmse_y']):.4f}\n"
    stats_text += f"  Max: {max(data['rmse_y']):.4f}\n"
    stats_text += f"  Mean: {np.mean(data['rmse_y']):.4f}\n"
    stats_text += f"  Final: {final_rmse['Y']:.4f}\n\n"
    
    stats_text += "X Velocity (m/s):\n"
    stats_text += f"  Min: {min(data['rmse_vx']):.4f}\n"
    stats_text += f"  Max: {max(data['rmse_vx']):.4f}\n"
    stats_text += f"  Mean: {np.mean(data['rmse_vx']):.4f}\n"
    stats_text += f"  Final: {final_rmse['Vx']:.4f}\n"
    
    ax5.text(0.05, 0.95, stats_text, transform=ax5.transAxes, fontsize=9,
            verticalalignment='top', fontfamily='monospace',
            bbox=dict(boxstyle='round', facecolor='lightblue', alpha=0.5))
    
    # ========== Plot 6: Status ==========
    ax6 = plt.subplot(2, 3, 6)
    ax6.axis('off')
    
    # Check if thresholds passed
    thresholds = [0.30, 0.16, 0.95, 0.70]
    values = [final_rmse['X'], final_rmse['Y'], final_rmse['Vx'], final_rmse['Vy']]
    passed = all(v <= t for v, t in zip(values, thresholds))
    
    status_color = 'lightgreen' if passed else 'lightcoral'
    status_text = "PASSED ✓" if passed else "FAILED ✗"
    
    status_box = "TEST RESULT\n" + "="*40 + "\n\n"
    status_box += f"Overall Status: {status_text}\n\n"
    
    status_box += "Threshold Check:\n"
    metrics = ['X Position', 'Y Position', 'X Velocity', 'Y Velocity']
    for metric, val, thresh in zip(metrics, values, thresholds):
        check = "✓ PASS" if val <= thresh else "✗ FAIL"
        status_box += f"  {metric}: {check}\n"
        status_box += f"    Value: {val:.4f}, Threshold: {thresh}\n"
    
    ax6.text(0.05, 0.95, status_box, transform=ax6.transAxes, fontsize=9,
            verticalalignment='top', fontfamily='monospace',
            bbox=dict(boxstyle='round', facecolor=status_color, alpha=0.7))
    
    plt.tight_layout()
    plt.show()

if __name__ == '__main__':
    output_path = r'c:\Users\TonyS\LaneSense\highway-kalman-filter\output.txt'
    
    print("Parsing UKF output file...")
    data, final_rmse, content = parse_output_file(output_path)
    
    if data:
        print(f"\nFound {len(data['times'])} time steps")
        print(f"Final RMSE: X={final_rmse['X']:.4f}m, Y={final_rmse['Y']:.4f}m")
        print(f"           Vx={final_rmse['Vx']:.4f}m/s, Vy={final_rmse['Vy']:.4e}m/s")
        print("\nGenerating visualization...")
        create_visualization(data, final_rmse, content)
    else:
        print("Failed to parse output file")
